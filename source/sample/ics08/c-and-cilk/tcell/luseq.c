#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double rtime = 0.0;

double
elapsed_time(struct timeval tp[2])
{
  return tp[1].tv_sec-tp[0].tv_sec+1e-6*(tp[1].tv_usec-tp[0].tv_usec);
}


/*
  (0,0)     ...   (0, nx-1)
  (1,0)     ...   (1, nx-1)
   ...             ...
  (ny-1,0)  ...   (ny-1, nx-1)
*/

/* eventually, may use different nx(s) */

void decomp_lu(int n, double *a, int nx);
void decomp_r(int n1, int n2, double *b, double *a, int nx);
void decomp_d(int n1, int n2, double *b, double *a, int nx);
void decomp_rd(int n1, int n2, int n3, double *b1, double *b2, 
	       double *a, int nx);

void
decomp_lu_0(int n, double *a, int nx)
{
  int i,j,k;
#if 0
  printf("lu_0: %d %d\n", n, a);
#endif
  {
    double w = 1.0/a[0];
    for(j=1;j<n;j++) a[j] *= w;
  }
  for(i=1;i<n;i++)
    {
      for(k=0;k<i-1;k++)
	{
	  double aik = a[i*nx+k];
	  for(j=k+1;j<n;j++) a[i*nx+j] -= aik * a[k*nx+j];
	}
      {
	double aik = a[i*nx+i-1];
	double w;
	a[i*nx+i] -= aik * a[(i-1)*nx+i];
	w = 1.0/a[i*nx+i];
	for(j=i+1;j<n;j++) a[i*nx+j] = w * (a[i*nx+j] - aik * a[(i-1)*nx+j]);
      }
    }
}

void
decomp_lu(int n, double *a, int nx)
{
  struct timeval tp[2];
  
  if(n <= 4)
    {
      decomp_lu_0(n, a, nx);
      return;
    }
  {
    /* int n1 = (n/4), n2 = n-n1; */
    /* int n1 = (n>8)?(n/4):(n/2), n2 = n-n1; */
    int n1 = (n>16)?(n/4):(n/2), n2 = n-n1;
    decomp_lu(n1, a, nx);
    // seq
#if 0
    gettimeofday(tp, 0);
#endif
    decomp_r(n1, n2, a, a+n1, nx);
#if 0
    gettimeofday(tp+1, 0);
    rtime += elapsed_time(tp);
#endif

    // par
    decomp_d(n1, n2, a, a+(n1*nx), nx);
    // seq
    decomp_rd(n1, n2, n2, a+(n1*nx), a+n1, a+((n1*nx)+n1), nx);
    // seq
    decomp_lu(n2, a+((n1*nx)+n1), nx);
  }
}


void
decomp_r_0(int n1, int n2, double *b, double *a, int nx)
{
  int i,j,k;
#if 0
  printf("r_0: %d %d %d %d\n", n1, n2, b, a);
#endif
  {
    double w = 1.0/b[0];
    for(j=0;j<n2;j++) a[j] *= w;
  }
  for(i=1;i<n1;i++)
    {
      for(k=0;k<i-1;k++)
	{
	  double aik = b[i*nx+k];
	  for(j=0;j<n2;j++) a[i*nx+j] -= aik * a[k*nx+j];
	}
      {
	double aik = b[i*nx+i-1];
	double w = 1.0/b[i*nx+i];
	for(j=0;j<n2;j++) a[i*nx+j] = w * (a[i*nx+j] - aik * a[(i-1)*nx+j]);
      }
    }


}

void
decomp_r(int n1, int n2, double *b, double *a, int nx)
{
  if(n1 <= 4)
    {
      decomp_r_0(n1, n2, b, a, nx);
      return;
    }
  {
    int n1_1 = n1/2, n1_2 = n1-n1_1;
    decomp_r(n1_1, n2, b, a, nx);
    // seq
    decomp_rd(n1_1, n1_2, n2, b+(n1_1*nx), a, a+(n1_1*nx), nx);
    // seq
    decomp_r(n1_2, n2, b+(n1_1*nx+n1_1), a+(n1_1*nx), nx);
  }
}

void
decomp_d_0(int n1, int n2, double *b, double *a, int nx)
{
  int i,j,k;
#if 0
  printf("d_0: %d %d %d %d\n", n1, n2, b, a);
#endif
  for(i=0;i<n2;i++)
    {
      for(k=0;k<n1-1;k++)
	{
	  double aik = a[i*nx+k];
	  for(j=k+1;j<n1;j++) a[i*nx+j] -= aik * b[k*nx+j];
	}

    }
}

void
decomp_d(int n1, int n2, double *b, double *a, int nx)
{
  if(n2 <= 4)
    {
      decomp_d_0(n1, n2, b, a, nx);
      return;
    }
  {
    int n2_1 = n2/2, n2_2 = n2-n2_1;
    decomp_d(n1, n2_1, b, a, nx);
    // par
    decomp_d(n1, n2_2, b, a+(n2_1*nx), nx);
  }
}

void
decomp_d_recur(int n1, int n2, double *b, double *a, int nx)
{
  if(n2 <= 4)
    {
      decomp_d_0(n1, n2, b, a, nx);
      return;
    }
  if(n1 > n2*4)
    {
      int n1_1 = n1/2, n1_2 = n1-n1_1;
      decomp_d(n1_1, n2, b, a, nx);
      // seq
      decomp_rd(n1_1, n2, n1_2, a, b+n1_1, a+n1_1, nx);
      // seq
      decomp_d(n1_2, n2, b+(n1_1*nx+n1_1), a+n1_1, nx);
    }
  else
    {
      int n2_1 = n2/2, n2_2 = n2-n2_1;
      decomp_d(n1, n2_1, b, a, nx);
      // par
      decomp_d(n1, n2_2, b, a+(n2_1*nx), nx);
    }
}

void
decomp_rd_0(int n1, int n2, int n3, 
	    double *b1, double *b2, double *a, int nx)
{
  int i,j,k;
#if 0
  printf("rd_0: %d %d %d %d %d %d\n", n1, n2, n3, b1, b2, a);
#endif
  for(i=0;i<n2;i++)
    for(k=0;k<n1;k++)
      {
	double aik = b1[i*nx+k];
	for(j=0;j<n3;j++)
	  a[i*nx+j] -= aik * b2[k*nx+j];
      }
}

void
decomp_rd_tile(int n1, int n2, int n3, 
	    double *b1, double *b2, double *a, int nx)
{
  int bs=32, i0, k0;
  int i,j,k;
  for(i0=0;i0<n2;i0+=bs)
    for(k0=0;k0<n1;k0+=bs)
      for(i=i0;i<i0+bs && i<n2;i++)
	for(k=k0;k<k0+bs && k<n1;k++)
	  {
	    double aik = b1[i*nx+k];
	    for(j=0;j<n3;j++)
	      a[i*nx+j] -= aik * b2[k*nx+j];
	  }
}

void
decomp_rd(int n1, int n2, int n3, 
	  double *b1, double *b2, double *a, int nx)
{
  if(n1 <= 4 && n2 <= 4)
    {
      decomp_rd_0(n1, n2, n3, b1, b2, a, nx);
      return;
    }
  if(n1 > n2)
    {
      int n1_1 = n1/2, n1_2 = n1 - n1_1;
      decomp_rd(n1_1, n2, n3, b1, b2, a, nx);
      // seq (par w buf)
      decomp_rd(n1_2, n2, n3, b1+n1_1, b2+(n1_1*nx), a, nx);
    }
  else
    {
      int n2_1 = n2/2, n2_2 = n2 - n2_1;
      decomp_rd(n1, n2_1, n3, b1, b2, a, nx);
      // par
      decomp_rd(n1, n2_2, n3, b1+(n2_1*nx), b2, a+(n2_1*nx), nx);
    }
}


void
decomp_lu_1(int n, double *a, int nx)
{
  int i;
  for(i=0;i<n;i+=16){
    int n1 = 16;
    if (n1 > n-i) n1 = n-i;
    decomp_d_0(i, n1, a, a+(i*nx), nx);
    decomp_rd(i, n1, n-i, a+(i*nx), a+i, a+(i*nx+i), nx);
    decomp_lu_0(n1, a+(i*nx+i), nx);
    decomp_r_0(n1, n-i-n1, a+(i*nx+i), a+(i*nx+i+n1), nx);
  }
}

void
genmat(int n, double *a, int nx)
{
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      a[i*nx+j] = n - abs(i-j);
}

void
printmat(int n, double *a, int nx)
{
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      printf("%6.3lf%c", a[i*nx+j], (j==n-1) ? '\n' : ' ');
  putchar('\n');
}

void
copy_l(int n, double *a, double *l, int nx)
{
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      l[i*nx+j] = (i<j) ? 0.0 : a[i*nx+j];
}

void
copy_u(int n, double *a, double *u, int nx)
{
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      u[i*nx+j] = (i==j) ? 1.0 : (i>j) ? 0.0 : a[i*nx+j];
}

void
transpose(int n, double *a, int nx)
{
  int i,j;
  for(i=0;i<n;i++) 
    for(j=0;j<i;j++)
      {
	double t1 = a[i*nx+j], t2 = a[j*nx+i];
	a[i*nx+j] = t2, a[j*nx+i] = t1;
      }
}

void
matmul(int n, double *a, double *b, double *c, int nx)
{
  int bs=12;
  int i0,j0,i,j,k;
  transpose(n, b, nx);
  for(i0=0;i0<n;i0+=bs)
    for(j0=0;j0<n;j0+=bs)
      for(i=i0;i<i0+bs && i<n;i++)
	for(j=j0;j<j0+bs && j<n;j++)
	  {
	    double s = 0.0;
	    for(k=0;k<n;k++) s += a[i*nx+k] * b[j*nx+k];
	    c[i*nx+j] = s;
	  }
}

void
diffmat(int n, double *a, double *b, int nx)
{
  double s=0.0;
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      s += (a[i*nx+j]-b[i*nx+j])*(a[i*nx+j]-b[i*nx+j]);
  printf("diff: %lf\n", s);
}

main(int argc, char *argv[])
{
  struct timeval tp[2];
  int n = 10, al = 1, d = 0, nx;
  double *a, *l, *u, *c;
  if(argc > 1) n = atol(argv[1]);
  if(argc > 2) al = atol(argv[2]);
  if(argc > 3) d = atol(argv[3]);
  nx = n;
  a = (double *)malloc(sizeof(double)*n*n);
  genmat(n, a, nx);
  if(d>1) printmat(n, a, nx);

  gettimeofday(tp, 0);
  switch(al){
  case 1: decomp_lu_0(n, a, nx); break;
  case 2: decomp_lu(n, a, nx); break;
  case 3: decomp_lu_1(n, a, nx); break;
  }
  gettimeofday(tp+1, 0);

  printf("%lf\n", elapsed_time(tp));

  if(d>0){
    l = (double *)malloc(sizeof(double)*n*n);
    u = (double *)malloc(sizeof(double)*n*n);
    c = (double *)malloc(sizeof(double)*n*n);
    if(d>1)printmat(n, a, nx);
    copy_l(n, a, l, nx);
    copy_u(n, a, u, nx);
    if(d>1)printmat(n, l, nx);
    if(d>1)printmat(n, u, nx);
    matmul(n, l, u, c, nx);
    if(d>1)printmat(n, c, nx);
    genmat(n, a, nx);
    diffmat(n, a, c, nx);
  }
  printf("%lf\n", rtime);

}
