#include <pthread.h>
#include <stdio.h>

int connect_to (char *hostname, unsigned short port);

void close_socket (int socket);

int send_char (char, int);

int send_string (char *str, int socket);

int send_fmt_string (int socket, char *fmt_string, ...);

int send_binary (void *src, unsigned long elm_size, unsigned long n_elm,
                 int socket);

int receive_char (int socket);

char *receive_line (char *buf, int maxlen, int socket);

int receive_binary (void *dst, unsigned long elm_size, unsigned long n_elm,
                    int socket);
extern char *receive_buf;
extern char *receive_buf_p;
enum node
{ OUTSIDE = -1, INSIDE = -2, ANY = -3, PARENT = -4, FORWARD = -5, TERM = -99 };
enum command
{ TASK, RSLT, TREQ, NONE, BACK, RACK, DREQ, DATA, STAT, VERB, EXIT, WRNG };
extern char *cmd_strings[];
enum choose
{ CHS_RANDOM, CHS_ORDER };

struct cmd
{
  enum command w;
  int c;
  enum node node;
  enum node v[4][16];
};

struct cmd_list
{
  struct cmd cmd;
  void *body;
  int task_no;
  struct cmd_list *next;
};
struct task;
struct thread_data;
void (*task_doers[256]) (struct thread_data *, void *);
void (*task_senders[256]) (void *);
void *(*task_receivers[256]) ();
void (*rslt_senders[256]) (void *);
void (*rslt_receivers[256]) (void *);

void data_allocate (int);

void data_send (int, int);

void data_receive (int, int);
struct worker_data;

void worker_init (struct thread_data *);

void _setup_data (int);

void _request_data (struct thread_data *, int, int);

void _wait_data (int, int);

void _set_exist_flag (int, int);
enum task_stat
{ TASK_ALLOCATED, TASK_INITIALIZED, TASK_STARTED, TASK_DONE, TASK_NONE,
    TASK_SUSPENDED };
enum task_home_stat
{ TASK_HOME_ALLOCATED, TASK_HOME_INITIALIZED, TASK_HOME_DONE };

struct task
{
  enum task_stat stat;
  struct task *next;
  struct task *prev;
  int task_no;
  void *body;
  int ndiv;
  enum node rslt_to;
  enum node rslt_head[16];
};

struct task_home
{
  enum task_home_stat stat;
  int id;
  struct task *owner;
  int task_no;
  enum node req_from;
  struct task_home *next;
  void *body;
  enum node task_head[16];
};

struct thread_data
{
  struct task_home *req;
  int id;
  int w_rack;
  int w_none;
  int ndiv;
  int last_treq;
  enum choose last_choose;
  double random_seed1;
  double random_seed2;
  struct task *task_free;
  struct task *task_top;
  struct task_home *treq_free;
  struct task_home *treq_top;
  struct task_home *sub;
  pthread_mutex_t mut;
  pthread_mutex_t rack_mut;
  pthread_cond_t cond;
  pthread_cond_t cond_r;
  void *wdptr;
  char dummy[1000];
};
enum DATA_FLAG
{ DATA_NONE, DATA_REQUESTING, DATA_EXIST };

struct dhandler_arg
{
  enum node data_to;
  enum node head[16];
  struct cmd dreq_cmd;
  struct cmd dreq_cmd_fwd;
  int start;
  int end;
};

void make_and_send_task (struct thread_data *thr, int task_no, void *body);

void *wait_rslt (struct thread_data *thr);

void proto_error (char const *str, struct cmd *pcmd);

void read_to_eol (void);

void init_data_flag (int);

void recv_rslt (struct cmd *, void *);

void recv_task (struct cmd *, void *);

void recv_treq (struct cmd *);

void recv_rack (struct cmd *);

void recv_dreq (struct cmd *);

void recv_data (struct cmd *);

void recv_none (struct cmd *);

void recv_back (struct cmd *);

void print_task_list (struct task *task_top, char *name);

void print_task_home_list (struct task_home *treq_top, char *name);

void print_thread_status (struct thread_data *thr);

void print_status (struct cmd *);

void set_verbose_level (struct cmd *);

void recv_exit (struct cmd *);

int serialize_cmdname (char *buf, enum command w);

int deserialize_cmdname (enum command *buf, char *str);

int serialize_arg (char *buf, enum node *arg);

enum node deserialize_node (char *str);

int deserialize_arg (enum node *buf, char *str);

int serialize_cmd (char *buf, struct cmd *pcmd);

int deserialize_cmd (struct cmd *pcmd, char *str);

int copy_address (enum node *dst, enum node *src);

int address_equal (enum node *adr1, enum node *adr2);

struct runtime_option
{
  int num_thrs;
  char sv_hostname[256];
  unsigned short port;
  char *initial_task;
  int auto_exit;
  int affinity;
  int prefetch;
  int verbose;
};
extern struct runtime_option option;

void handle_req (int (*)lightweight (void), struct thread_data *);

#include<sys/time.h>

int printf (char const *, ...);

int fprintf (FILE *, char const *, ...);

void *malloc (size_t);

void free (void *);

double sqrt (double);

double fabs (double);

int connect_to (char *hostname, unsigned short port);

void close_socket (int socket);

int send_char (char, int);

int send_string (char *str, int socket);

int send_fmt_string (int socket, char *fmt_string, ...);

int send_binary (void *src, unsigned long elm_size, unsigned long n_elm,
                 int socket);

int receive_char (int socket);

char *receive_line (char *buf, int maxlen, int socket);

int receive_binary (void *dst, unsigned long elm_size, unsigned long n_elm,
                    int socket);
extern char *receive_buf;
extern char *receive_buf_p;

double elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0E-6 * ((tp[1]).tv_usec -
                                                     (tp[0]).tv_usec);
}

void decomp_lu (int (*_bk) lightweight (void), struct thread_data *_thr, int n,
                double *a, int nx, int anx);

void decomp_r (int (*_bk) lightweight (void), struct thread_data *_thr, int n1,
               int n2, double *b, double *a, int nx, int an);

void decomp_d (int (*_bk) lightweight (void), struct thread_data *_thr, int n1,
               int n2, double *b, double *a, int nx, int an);

void decomp_rd (int (*_bk) lightweight (void), struct thread_data *_thr, int n1,
                int n2, int n3, double *b1, double *b2, double *a, int nx,
                int anx);

void zero_mat (double *a, int n1, int n2, int nx);
enum task_tp2
{ TASK_CMD, TASK_LU, TASK_R, TASK_D, TASK_RD };

struct task_lu
{
  enum task_tp2 tt;
  int n1;
  int n2;
  int n3;
  double *b1;
  double *b2;
  double *a;
  int nx;
  int anx;
  int tmpnx;
  char _dummy_[1000];
};

void put_task (struct task_lu *st, enum task_tp2 tt, int n1, int n2, int n3,
               double *b1, double *b2, double *a, int nx, int anx)
{
  (*st).tt = tt;
  (*st).n1 = n1;
  (*st).n2 = n2;
  (*st).n3 = n3;
  (*st).b1 = b1;
  (*st).b2 = b2;
  (*st).a = a;
  (*st).nx = nx;
  (*st).anx = anx;
  (*st).tmpnx = 0;
  if (a == 0)
    {
      (*st).tmpnx = n3;
      (*st).a = (double *) malloc (sizeof (double) * n2 * n3);
      zero_mat ((*st).a, n2, n3, n3);
    }
  else;
}

void merge_task (struct task_lu *x, double *a, int anx)
{
  int n2 = (*x).n2;
  int n3 = (*x).n3;
  double *tmp = (*x).a;
  int i;
  int j;

  if (a == tmp)
    return;
  else;
  {
    i = 0;
    for (; i < n2; i++)
      {
        j = 0;
        for (; j < n3; j++)
          {
            a[i * anx + j] += tmp[i * n3 + j];
          }
      }
  }
  free (tmp);
}

void decomp_lu_0 (int n, double *a, int nx, int anx)
{
  int i;
  int j;
  int k;

  {
    double w = 1.0 / a[0];

    {
      j = 1;
      for (; j < n; j++)
        {
          a[j] *= w;
        }
    }
  }
  {
    i = 1;
    for (; i < n; i++)
      {
        {
          k = 0;
          for (; k < i - 1; k++)
            {
              double aik = a[i * anx + k];

              {
                j = k + 1;
                for (; j < n; j++)
                  {
                    a[i * anx + j] -= aik * a[k * anx + j];
                  }
              }
            }
        }
        {
          double aik = a[i * anx + i - 1];
          double w;

          a[i * anx + i] -= aik * a[(i - 1) * anx + i];
          w = 1.0 / a[i * anx + i];
          {
            j = i + 1;
            for (; j < n; j++)
              {
                a[i * anx + j] =
                  w * (a[i * anx + j] - aik * a[(i - 1) * anx + j]);
              }
          }
        }
      }
  }
}

void decomp_lu (int (*_bk) lightweight (void), struct thread_data *_thr, int n,
                double *a, int nx, int anx)
{
  if (n <= 4)
    {
      decomp_lu_0 (n, a, nx, anx);
      return;
    }
  else;
  {
    int n1 = n > 16 ? n / 4 : n / 2;
    int n2 = n - n1;

    decomp_lu (_bk, _thr, n1, a, nx, anx);
    {
      struct task_lu st2;
      struct task_lu *pthis = &st2;
      int spawned = 0;

      {

        int do_two_bk lightweight (void)
        {
          if (spawned)
            return 0;
          else;
          _bk ();
          if ((*_thr).treq_top)
            {
              put_task (&(*pthis), TASK_D, n1, n2, 0, a, 0, a + n1 * nx, nx,
                        anx);
              spawned = 1;
              make_and_send_task (_thr, 0, pthis);
              return 1;
            }
          else;
          return 0;
        }
        if ((*_thr).req)
          handle_req (do_two_bk, _thr);
        else;
        {
          decomp_r (do_two_bk, _thr, n1, n2, a, a + n1, nx, anx);
        }
      }
      if (spawned)
        {
          wait_rslt (_thr);
        }
      else
        {
          decomp_d (_bk, _thr, n1, n2, a, a + n1 * nx, nx, anx);
        }
    }
    decomp_rd (_bk, _thr, n1, n2, n2, a + n1 * nx, a + n1, a + n1 * nx + n1, nx,
               anx);
    decomp_lu (_bk, _thr, n2, a + n1 * nx + n1, nx, anx);
  }
}

void decomp_r_0 (int n1, int n2, double *b, double *a, int nx, int anx)
{
  int i;
  int j;
  int k;

  {
    double w = 1.0 / b[0];

    {
      j = 0;
      for (; j < n2; j++)
        {
          a[j] *= w;
        }
    }
  }
  {
    i = 1;
    for (; i < n1; i++)
      {
        {
          k = 0;
          for (; k < i - 1; k++)
            {
              double aik = b[i * nx + k];

              {
                j = 0;
                for (; j < n2; j++)
                  {
                    a[i * anx + j] -= aik * a[k * anx + j];
                  }
              }
            }
        }
        {
          double aik = b[i * nx + i - 1];
          double w = 1.0 / b[i * nx + i];

          {
            j = 0;
            for (; j < n2; j++)
              {
                a[i * anx + j] =
                  w * (a[i * anx + j] - aik * a[(i - 1) * anx + j]);
              }
          }
        }
      }
  }
}

void decomp_r (int (*_bk) lightweight (void), struct thread_data *_thr, int n1,
               int n2, double *b, double *a, int nx, int anx)
{
  if (n1 <= 4)
    {
      decomp_r_0 (n1, n2, b, a, nx, anx);
      return;
    }
  else;
  {
    int n1_1 = n1 / 2;
    int n1_2 = n1 - n1_1;

    decomp_r (_bk, _thr, n1_1, n2, b, a, nx, anx);
    decomp_rd (_bk, _thr, n1_1, n1_2, n2, b + n1_1 * nx, a, a + n1_1 * nx, nx,
               anx);
    decomp_r (_bk, _thr, n1_2, n2, b + (n1_1 * nx + n1_1), a + n1_1 * nx, nx,
              anx);
  }
}

void decomp_d_0 (int n1, int n2, double *b, double *a, int nx, int anx)
{
  int i;
  int j;
  int k;

  {
    i = 0;
    for (; i < n2; i++)
      {
        k = 0;
        for (; k < n1 - 1; k++)
          {
            double aik = a[i * anx + k];

            {
              j = k + 1;
              for (; j < n1; j++)
                {
                  a[i * anx + j] -= aik * b[k * nx + j];
                }
            }
          }
      }
  }
}

void decomp_d (int (*_bk) lightweight (void), struct thread_data *_thr, int n1,
               int n2, double *b, double *a, int nx, int anx)
{
  if (n2 <= 4)
    {
      decomp_d_0 (n1, n2, b, a, nx, anx);
      return;
    }
  else;
  {
    int n2_1 = n2 / 2;
    int n2_2 = n2 - n2_1;

    {
      struct task_lu st3;
      struct task_lu *pthis = &st3;
      int spawned2 = 0;

      {

        int do_two_bk2 lightweight (void)
        {
          if (spawned2)
            return 0;
          else;
          _bk ();
          if ((*_thr).treq_top)
            {
              put_task (&(*pthis), TASK_D, n1, n2_2, 0, b, 0, a + n2_1 * nx, nx,
                        anx);
              spawned2 = 1;
              make_and_send_task (_thr, 0, pthis);
              return 1;
            }
          else;
          return 0;
        }
        if ((*_thr).req)
          handle_req (do_two_bk2, _thr);
        else;
        {
          decomp_d (do_two_bk2, _thr, n1, n2_1, b, a, nx, anx);
        }
      }
      if (spawned2)
        {
          wait_rslt (_thr);
        }
      else
        {
          decomp_d (_bk, _thr, n1, n2_2, b, a + n2_1 * nx, nx, anx);
        }
    }
  }
}

void decomp_rd_0 (int n1, int n2, int n3, double *b1, double *b2, double *a,
                  int nx, int anx)
{
  int i;
  int j;
  int k;

  {
    i = 0;
    for (; i < n2; i++)
      {
        k = 0;
        for (; k < n1; k++)
          {
            double aik = b1[i * nx + k];

            {
              j = 0;
              for (; j < n3; j++)
                {
                  a[i * anx + j] -= aik * b2[k * nx + j];
                }
            }
          }
      }
  }
}

void decomp_rd (int (*_bk) lightweight (void), struct thread_data *_thr, int n1,
                int n2, int n3, double *b1, double *b2, double *a, int nx,
                int anx)
{
  if (n1 <= 4 && n2 <= 4)
    {
      decomp_rd_0 (n1, n2, n3, b1, b2, a, nx, anx);
      return;
    }
  else;
  if (n1 > n2)
    {
      int n1_1 = n1 / 2;
      int n1_2 = n1 - n1_1;

      decomp_rd (_bk, _thr, n1_1, n2, n3, b1, b2, a, nx, anx);
      decomp_rd (_bk, _thr, n1_2, n2, n3, b1 + n1_1, b2 + n1_1 * nx, a, nx,
                 anx);
    }
  else
    {
      int n2_1 = n2 / 2;
      int n2_2 = n2 - n2_1;

      {
        struct task_lu st4;
        struct task_lu *pthis = &st4;
        int spawned3 = 0;

        {

          int do_two_bk3 lightweight (void)
          {
            if (spawned3)
              return 0;
            else;
            _bk ();
            if ((*_thr).treq_top)
              {
                put_task (&(*pthis), TASK_RD, n1, n2_2, n3, b1 + n2_1 * nx, b2,
                          a + n2_1 * anx, nx, anx);
                spawned3 = 1;
                make_and_send_task (_thr, 0, pthis);
                return 1;
              }
            else;
            return 0;
          }
          if ((*_thr).req)
            handle_req (do_two_bk3, _thr);
          else;
          {
            decomp_rd (do_two_bk3, _thr, n1, n2_1, n3, b1, b2, a, nx, anx);
          }
        }
        if (spawned3)
          {
            wait_rslt (_thr);
            merge_task (&(*pthis), a + n2_1 * anx, anx);
          }
        else
          {
            decomp_rd (_bk, _thr, n1, n2_2, n3, b1 + n2_1 * nx, b2,
                       a + n2_1 * anx, nx, anx);
          }
      }
    }
}

void decomp_lu_1 (int (*_bk) lightweight (void), struct thread_data *_thr,
                  int n, double *a, int nx, int anx)
{
  int i;

  {
    i = 0;
    for (; i < n; i += 16)
      {
        int n1 = 16;

        if (n1 > n - i)
          n1 = n - i;
        else;
        decomp_d_0 (i, n1, a, a + i * nx, nx, anx);
        decomp_rd (_bk, _thr, i, n1, n - i, a + i * nx, a + i, a + (i * nx + i),
                   nx, anx);
        decomp_lu_0 (n1, a + (i * nx + i), nx, anx);
        decomp_r_0 (n1, n - i - n1, a + (i * nx + i), a + (i * nx + i + n1), nx,
                    anx);
      }
  }
}

void genmat (int n, double *a, int nx)
{
  int i;
  int j;

  {
    i = 0;
    for (; i < n; i++)
      {
        j = 0;
        for (; j < n; j++)
          {
            a[i * nx + j] = n - abs (i - j);
          }
      }
  }
}

void printmat (int n, double *a, int nx)
{
  int i;
  int j;

  {
    i = 0;
    for (; i < n; i++)
      {
        j = 0;
        for (; j < n; j++)
          {
            fprintf (stderr, "%6.3lf%c", a[i * nx + j],
                     j == n - 1 ? '\n' : ' ');
          }
      }
  }
  putc ('\n', stderr);
}

void copy_l (int n, double *a, double *l, int nx)
{
  int i;
  int j;

  {
    i = 0;
    for (; i < n; i++)
      {
        j = 0;
        for (; j < n; j++)
          {
            l[i * nx + j] = i < j ? 0.0 : a[i * nx + j];
          }
      }
  }
}

void copy_u (int n, double *a, double *u, int nx)
{
  int i;
  int j;

  {
    i = 0;
    for (; i < n; i++)
      {
        j = 0;
        for (; j < n; j++)
          {
            u[i * nx + j] = i == j ? 1.0 : (i > j ? 0.0 : a[i * nx + j]);
          }
      }
  }
}

void transpose (int n, double *a, int nx)
{
  int i;
  int j;

  {
    i = 0;
    for (; i < n; i++)
      {
        j = 0;
        for (; j < i; j++)
          {
            double t1 = a[i * nx + j];
            double t2 = a[j * nx + i];

            a[i * nx + j] = t2, a[j * nx + i] = t1;
          }
      }
  }
}

void matmul (int n, double *a, double *b, double *c, int nx)
{
  int bs = 12;
  int i0;
  int j0;
  int i;
  int j;
  int k;

  transpose (n, b, nx);
  {
    i0 = 0;
    for (; i0 < n; i0 += bs)
      {
        j0 = 0;
        for (; j0 < n; j0 += bs)
          {
            i = i0;
            for (; i < i0 + bs && i < n; i++)
              {
                j = j0;
                for (; j < j0 + bs && j < n; j++)
                  {
                    double s = 0.0;

                    {
                      k = 0;
                      for (; k < n; k++)
                        {
                          s += a[i * nx + k] * b[j * nx + k];
                        }
                    }
                    c[i * nx + j] = s;
                  }
              }
          }
      }
  }
}

void diffmat (int n, double *a, double *b, int nx)
{
  double s = 0.0;
  int i;
  int j;

  {
    i = 0;
    for (; i < n; i++)
      {
        j = 0;
        for (; j < n; j++)
          {
            s +=
              (a[i * nx + j] - b[i * nx + j]) * (a[i * nx + j] - b[i * nx + j]);
          }
      }
  }
  fprintf (stderr, "diff: %lf\n", s);
}

void input_mat (double *a, int n1, int n2, int nx)
{
  int i;
  int swp;

  swp = recv_binary_header (0, 0);
  {
    i = 0;
    for (; i < n1; i++)
      {
        recv_double_seq (a + i * nx, n2);
      }
  }
  recv_binary_terminator ();
  if (swp)
    {
      i = 0;
      for (; i < n1; i++)
        {
          swap_doubles (a + i * nx, n2);
        }
    }
  else;
}

void output_mat (double *a, int n1, int n2, int nx)
{
  int i;
  send_binary_header (sizeof (double), n1 * n2);
  {
    i = 0;
    for (; i < n1; i++)
      {
        send_double_seq (a + i * nx, n2);
      }
  }
  send_binary_terminator ();
}

void input_mat_l (double *a, int n1, int nx)
{
  int i;
  int swp;

  swp = recv_binary_header (0, 0);
  {
    i = 0;
    for (; i < n1; i++)
      {
        recv_double_seq (a + i * nx, i + 1);
      }
  }
  recv_binary_terminator ();
  if (swp)
    {
      i = 0;
      for (; i < n1; i++)
        {
          swap_doubles (a + i * nx, i + 1);
        }
    }
  else;
}

void output_mat_l (double *a, int n1, int nx)
{
  int i;
  send_binary_header (sizeof (double), n1 * (n1 + 1) / 2);
  {
    i = 0;
    for (; i < n1; i++)
      {
        send_double_seq (a + i * nx, i + 1);
      }
  }
  send_binary_terminator ();
}

void input_mat_u (double *a, int n1, int nx)
{
  int i;
  int swp;

  swp = recv_binary_header (0, 0);
  {
    i = 0;
    for (; i < n1; i++)
      {
        recv_double_seq (a + i * nx + i + 1, n1 - i - 1);
      }
  }
  recv_binary_terminator ();
  if (swp)
    {
      i = 0;
      for (; i < n1; i++)
        {
          swap_doubles (a + i * nx + i + 1, n1 - i - 1);
        }
    }
  else;
}

void output_mat_u (double *a, int n1, int nx)
{
  int i;
  send_binary_header (sizeof (double), n1 * (n1 - 1) / 2);
  {
    i = 0;
    for (; i < n1; i++)
      {
        send_double_seq (a + i * nx + i + 1, n1 - i - 1);
      }
  }
  send_binary_terminator ();
}

void zero_mat (double *a, int n1, int n2, int nx)
{
  int i;
  int j;

  {
    i = 0;
    for (; i < n1; i++)
      {
        j = 0;
        for (; j < n2; j++)
          {
            a[i * nx + j] = 0.0;
          }
      }
  }
}

void proc_rslt (double *a)
{
  printf ("%lf\n", *a);
}

void start_lu (int (*_bk) lightweight (void), struct thread_data *_thr, int n1,
               int n2, int n3, double *ansp)
{
  struct timeval tp[2];
  int n = n1;
  int al = n2;
  int d = n3;
  int nx;
  double *a;
  double *l;
  double *u;
  double *c;

  nx = n;
  a = (double *) malloc (sizeof (double) * n * n);
  genmat (n, a, nx);
  if (d > 1)
    printmat (n, a, nx);
  else;
  gettimeofday (tp, 0);
  switch (al)
    {
    case 1:
      decomp_lu_0 (n, a, nx, nx);
      break;
    case 2:
      decomp_lu (_bk, _thr, n, a, nx, nx);
      break;
    case 3:
      decomp_lu_1 (_bk, _thr, n, a, nx, nx);
      break;
    }
  gettimeofday (tp + 1, 0);
  *ansp = elapsed_time (tp);
  fprintf (stderr, "time: %lf\n", *ansp);
  if (d > 0)
    {
      l = (double *) malloc (n * n * sizeof (double));
      u = (double *) malloc (n * n * sizeof (double));
      c = (double *) malloc (n * n * sizeof (double));
      if (d > 1)
        printmat (n, a, nx);
      else;
      copy_l (n, a, l, nx);
      copy_u (n, a, u, nx);
      if (d > 1)
        printmat (n, l, nx);
      else;
      if (d > 1)
        printmat (n, u, nx);
      else;
      matmul (n, l, u, c, nx);
      if (d > 1)
        printmat (n, c, nx);
      else;
      genmat (n, a, nx);
      diffmat (n, a, c, nx);
      free (l);
      free (u);
      free (c);
    }
  else;
  free (a);
}

void do_task_lu_task (struct thread_data *_thr, struct task_lu *pthis)
{

  int _bk lightweight (void)
  {
    return 0;
  }
  int n1 = (*pthis).n1;
  int n2 = (*pthis).n2;
  int n3 = (*pthis).n3;
  double *b1 = (*pthis).b1;
  double *b2 = (*pthis).b2;
  double *a = (*pthis).a;
  int nx = (*pthis).nx;
  int anx = (*pthis).tmpnx ? (*pthis).tmpnx : (*pthis).anx;

  {
  }
  switch ((*pthis).tt)
    {
    case TASK_CMD:
      start_lu (_bk, _thr, n1, n2, n3, a);
      break;
    case TASK_LU:
      decomp_lu (_bk, _thr, n1, a, nx, anx);
      break;
    case TASK_R:
      decomp_r (_bk, _thr, n1, n2, b1, a, nx, anx);
      break;
    case TASK_D:
      decomp_d (_bk, _thr, n1, n2, b1, a, nx, anx);
      break;
    case TASK_RD:
      decomp_rd (_bk, _thr, n1, n2, n3, b1, b2, a, nx, anx);
      break;
    }
}

void data_allocate (int n1)
{
}

void data_send (int n1, int n2)
{
}

void data_receive (int n1, int n2)
{
}

void send_task_lu_task (struct task_lu *pthis)
{
  int n1 = (*pthis).n1;
  int n2 = (*pthis).n2;
  int n3 = (*pthis).n3;
  double *b1 = (*pthis).b1;
  double *b2 = (*pthis).b2;
  double *a = (*pthis).a;
  int nx = (*pthis).nx;
  int anx = (*pthis).anx;

  send_int ((*pthis).tt);
  switch ((*pthis).tt)
    {
    case TASK_LU:
      send_int (n1);
      output_mat (a, n1, n1, anx);
      break;
    case TASK_R:
      send_int (n1);
      send_int (n2);
      output_mat_l (b1, n1, nx);
      output_mat (a, n1, n2, anx);
      break;
    case TASK_D:
      send_int (n1);
      send_int (n2);
      output_mat_u (b1, n1, nx);
      output_mat (a, n2, n1, anx);
      break;
    case TASK_RD:
      send_int (n1);
      send_int (n2);
      send_int (n3);
      output_mat (b1, n2, n1, nx);
      output_mat (b2, n1, n3, nx);
      break;
    }
}

struct task_lu *recv_task_lu_task ()
{
  struct task_lu *pthis = malloc (sizeof (struct task_lu));
  enum task_tp2 tt;
  int n1;
  int n2;
  int n3;
  double *b1;
  double *b2;
  double *a;
  int nx;
  static double ans;

  tt = recv_int ();
  switch (tt)
    {
    case TASK_CMD:
      n1 = recv_int ();
      n2 = recv_int ();
      n3 = recv_int ();
      a = &ans;
      break;
    case TASK_LU:
      n1 = recv_int ();
      nx = n1;
      a = (double *) malloc (sizeof (double) * n1 * n1);
      input_mat (a, n1, n1, nx);
      break;
    case TASK_R:
      n1 = recv_int ();
      n2 = recv_int ();
      nx = n1 + n2;
      b1 = (double *) malloc (sizeof (double) * n1 * nx);
      input_mat_l (b1, n1, nx);
      a = b1 + n1;
      input_mat (a, n1, n2, nx);
      break;
    case TASK_D:
      n1 = recv_int ();
      n2 = recv_int ();
      nx = n1;
      b1 = (double *) malloc (sizeof (double) * (n1 + n2) * nx);
      input_mat_u (b1, n1, nx);
      a = b1 + n1 * nx;
      input_mat (a, n2, n1, nx);
      break;
    case TASK_RD:
      n1 = recv_int ();
      n2 = recv_int ();
      n3 = recv_int ();
      nx = n1 + n3;
      a = (double *) malloc (sizeof (double) * (n1 + n2) * nx);
      b1 = a + n1 * nx;
      b2 = a + n1;
      a += n1 * nx + n1;
      input_mat (b1, n2, n1, nx);
      input_mat (b2, n1, n3, nx);
      zero_mat (a, n2, n3, nx);
      break;
    }
  (*pthis).tt = tt;
  (*pthis).n1 = n1;
  (*pthis).n2 = n2;
  (*pthis).n3 = n3;
  (*pthis).b1 = b1;
  (*pthis).b2 = b2;
  (*pthis).a = a;
  (*pthis).nx = nx;
  (*pthis).anx = nx;
  (*pthis).tmpnx = 0;
  return pthis;
}

void send_task_lu_rslt (struct task_lu *pthis)
{
  int n1 = (*pthis).n1;
  int n2 = (*pthis).n2;
  int n3 = (*pthis).n3;
  double *b1 = (*pthis).b1;
  double *b2 = (*pthis).b2;
  double *a = (*pthis).a;
  int anx = (*pthis).anx;

  switch ((*pthis).tt)
    {
    case TASK_CMD:
      proc_rslt (a);
      break;
    case TASK_LU:
      output_mat (a, n1, n1, anx);
      free (a);
      break;
    case TASK_R:
      output_mat (a, n1, n2, anx);
      free (b1);
      break;
    case TASK_D:
      output_mat (a, n2, n1, anx);
      free (b1);
      break;
    case TASK_RD:
      output_mat (a, n2, n3, anx);
      a -= n1 * anx + n1;
      free (a);
      break;
    }
  free (pthis);
}

void recv_task_lu_rslt (struct task_lu *pthis)
{
  int n1 = (*pthis).n1;
  int n2 = (*pthis).n2;
  int n3 = (*pthis).n3;
  double *b1 = (*pthis).b1;
  double *b2 = (*pthis).b2;
  double *a = (*pthis).a;
  int tmpnx = (*pthis).tmpnx;
  int anx = (*pthis).anx;

  switch ((*pthis).tt)
    {
    case TASK_LU:
      input_mat (a, n1, n1, anx);
      break;
    case TASK_R:
      input_mat (a, n1, n2, anx);
      break;
    case TASK_D:
      input_mat (a, n2, n1, anx);
      break;
    case TASK_RD:
      if (tmpnx == 0)
        {
          tmpnx = n3;
          (*pthis).a = (double *) malloc (sizeof (double) * n2 * n3);
        }
      else;
      input_mat ((*pthis).a, n2, n3, tmpnx);
      break;
    }
}
void (*task_doers[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) do_task_lu_task};
void (*task_senders[256]) (void *) =
{
(void (*)(void *)) send_task_lu_task};
void *(*task_receivers[256]) () =
{
(void *(*)()) recv_task_lu_task};
void (*rslt_senders[256]) (void *) =
{
(void (*)(void *)) send_task_lu_rslt};
void (*rslt_receivers[256]) (void *) =
{
(void (*)(void *)) recv_task_lu_rslt};

struct worker_data
{
};

void worker_init (struct thread_data *_thr)
{
}
