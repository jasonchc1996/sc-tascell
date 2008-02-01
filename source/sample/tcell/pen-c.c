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
enum node
{ OUTSIDE, INSIDE, ANY };

struct cmd
{
  int c;
  enum node node;
  char *v[5];
};

struct cmd_list
{
  struct cmd cmd;
  void *body;
  int task_no;
  struct cmd_list *next;
};

void read_to_eol (void);

void recv_rslt (struct cmd *, void *);

void recv_task (struct cmd *, void *);

void recv_treq (struct cmd *);

void recv_rack (struct cmd *);

void recv_none (struct cmd *);
struct task;
struct thread_data;
void (*task_doers[256]) (struct thread_data *, void *);
void (*task_senders[256]) (void *);
void *(*task_receivers[256]) ();
void (*rslt_senders[256]) (void *);
void (*rslt_receivers[256]) (void *);
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
  char rslt_head[256];
};

struct task_home
{
  enum task_home_stat stat;
  int id;
  int task_no;
  enum node req_from;
  struct task_home *next;
  void *body;
  char task_head[256];
};

struct thread_data
{
  struct task_home *req;
  int id;
  int w_rack;
  int w_none;
  int ndiv;
  struct task *task_free;
  struct task *task_top;
  struct task_home *treq_free;
  struct task_home *treq_top;
  struct task_home *sub;
  pthread_mutex_t mut;
  pthread_mutex_t rack_mut;
  pthread_cond_t cond;
  pthread_cond_t cond_r;
  char ndiv_buf[32];
  char tno_buf[8];
  char id_str[32];
  char buf[256];
};
extern int divisibility_flag;

void send_divisible (void);

void make_and_send_task (struct thread_data *thr, int task_no, void *body);

void *wait_rslt (struct thread_data *thr);

void send_int (int n);

int recv_int (void);

void handle_req (int (*)(void), struct thread_data *);
#include<sys/time.h>

int printf (char const *, ...);

int fprintf (FILE *, char const *, ...);

void *malloc (size_t);

void free (void *);

void send_int (int n);

int recv_int (void);

int send_binary_header (int elmsize, int nelm);

int recv_binary_header (int *pelmsize, int *pnelm);

void send_binary_terminator (void);

void recv_binary_terminator (void);

int send_double_seq (double *a, int nelm);

int recv_double_seq (double *a, int nelm);

void swap_doubles (double *a, int n);

struct pentomino
{
  int r;
  int k;
  int i0;
  int i1;
  int i2;
  int a[12];
  int b[70];
};

int search (int k, int j0, int j1, int j2, struct pentomino *tsk);

void
do_pentomino_task (struct thread_data *_thr, struct pentomino *pthis)
{
  int i;
  int k = (*pthis).k;
  int i0 = (*pthis).i0;
  int i1 = (*pthis).i1;
  int i2 = (*pthis).i2;
  fprintf (stderr, "start %d %d %d %d\n", k, i0, i1, i2);
  (*pthis).r = search (k, i0, i1, i2, &(*pthis));
  fprintf (stderr, "end %d\n", (*pthis).r);
}

struct start_pentomino
{
  int r;
};

double
elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0e-6 * ((tp[1]).tv_usec -
                                                     (tp[0]).tv_usec);
}

void
do_start_pentomino_task (struct thread_data *_thr,
                         struct start_pentomino *pthis)
{
  struct timeval tp[2];
  struct pentomino pen;
  int i;
  {
    i = 0;
    for (; i < 12; i++)
      {
        (pen.a)[i] = i;
      }
  }
  {
    i = 0;
    for (; i < 70; i++)
      {
        (pen.b)[i] = 0;
      }
  }
  {
    i = 6;
    for (; i < 70; i += 7)
      {
        (pen.b)[i] = '\n';
      }
  }
  gettimeofday (tp, 0);
  (*pthis).r = search (0, 0, 0, 12, &pen);
  gettimeofday (tp + 1, 0);
  fprintf (stderr, "time: %lf\n", elapsed_time (tp));
}
int ps[57][5] =
  { {1, 1, 1, 1}, {7, 7, 7, 7}, {4, 1, 1, 1}, {7, 7, 6, 1}, {6, 1, 1, 1}, {5,
                                                                           1,
                                                                           1,
                                                                           1},
  {1, 1, 1, 5}, {1, 1, 1, 6}, {7, 1, 6, 7}, {7, 7, 1, 6}, {7, 6, 1, 7}, {6, 1,
                                                                         7,
                                                                         7},
  {7, 7, 1, 1}, {1, 1, 5, 7}, {1, 1, 7, 7}, {7, 5, 1, 1}, {2, 5, 1, 1}, {1, 1,
                                                                         5,
                                                                         2},
  {1, 6, 7, 1}, {1, 7, 6, 1}, {7, 1, 1, 5}, {5, 1, 1, 7}, {7, 6, 1, 1}, {1, 1,
                                                                         6,
                                                                         7},
  {7, 1, 1, 6}, {5, 1, 1, 6}, {6, 1, 1, 5}, {6, 1, 1, 7}, {1, 7, 1, 6}, {7, 1,
                                                                         5,
                                                                         1},
  {1, 5, 1, 7}, {6, 1, 7, 1}, {7, 1, 1, 7}, {5, 1, 1, 5}, {1, 7, 7, 1}, {1, 6,
                                                                         6,
                                                                         1},
  {6, 1, 1, 6}, {1, 6, 1, 1}, {1, 1, 5, 1}, {1, 5, 1, 1}, {1, 1, 6, 1}, {7, 1,
                                                                         6,
                                                                         1},
  {1, 6, 1, 6}, {6, 1, 6, 1}, {1, 6, 1, 7}, {1, 4, 1, 1}, {1, 1, 7, 1}, {1, 7,
                                                                         1,
                                                                         1},
  {1, 1, 4, 1}, {7, 1, 7, 7}, {7, 6, 1, 6}, {6, 1, 6, 7}, {7, 7, 1, 7}, {7, 1,
                                                                         7,
                                                                         1},
  {1, 5, 1, 6}, {1, 7, 1, 7}, {6, 1, 5, 1} };
int pos[13] = { 0, 2, 4, 12, 16, 20, 24, 32, 36, 37, 45, 53, 57 };

int
search (int k, int j0, int j1, int j2, struct pentomino *tsk)
{
  int s = 0;
  {
    int p = j1;
    int p_end = j2;
    for (; p < p_end; p++)
      {
        int ap = ((*tsk).a)[p];
        int i;
        {
          i = pos[ap];
          for (; i < pos[ap + 1]; i++)
            {
              int *pss = ps[i];
              int kk = k;
              int l;
              {
                l = 0;
                for (; l < 4; l++)
                  {
                    if ((kk += pss[l]) >= 70 || ((*tsk).b)[kk] != 0)
                      goto Ln;
                  }
              }
              {
                {
                  ((*tsk).b)[kk = k] = p + 'A';
                  {
                    l = 0;
                    for (; l < 4; l++)
                      {
                        ((*tsk).b)[kk += pss[l]] = p + 'A';
                      }
                  }
                  ((*tsk).a)[p] = ((*tsk).a)[j0];
                  ((*tsk).a)[j0] = ap;
                }
                {
                  {
                    {
                      kk = k;
                      for (; kk < 70; kk++)
                        {
                          if (((*tsk).b)[kk] == 0)
                            break;
                        }
                    }
                    if (kk == 70)
                      {
                        s += 1;
                      }
                    else if ((kk + 7 >= 70 || ((*tsk).b)[kk + 7] != 0)
                             && (((*tsk).b)[kk + 1] != 0
                                 || (kk + 8 >= 70 || ((*tsk).b)[kk + 8] != 0)
                                 && ((*tsk).b)[kk + 2] != 0))
                      {
                        ;
                      }
                    else
                      {
                        s += search (kk, j0 + 1, j0 + 1, 12, tsk);
                      }
                  }
                  {
                    ap = ((*tsk).a)[j0];
                    ((*tsk).a)[j0] = ((*tsk).a)[p];
                    ((*tsk).a)[p] = ap;
                    ((*tsk).b)[kk = k] = 0;
                    {
                      l = 0;
                      for (; l < 4; l++)
                        {
                          ((*tsk).b)[kk += pss[l]] = 0;
                        }
                    }
                  }
                }
              }
            Ln:continue;
            }
        }
      }
  }
  return s;
}

void
send_start_pentomino_task (struct start_pentomino *pthis)
{
}

struct start_pentomino *
recv_start_pentomino_task ()
{
  struct start_pentomino *pthis = malloc (sizeof (struct start_pentomino));
  return pthis;
}

void
send_start_pentomino_rslt (struct start_pentomino *pthis)
{
  send_int ((*pthis).r);
  free (pthis);
}

void
recv_start_pentomino_rslt (struct start_pentomino *pthis)
{
  (*pthis).r = recv_int ();
}

void
send_pentomino_task (struct pentomino *pthis)
{
  send_int ((*pthis).k);
  send_int ((*pthis).i0);
  send_int ((*pthis).i1);
  send_int ((*pthis).i2);
  {
    int i3;
    int sz = 12;
    for (i3 = 0; i3 < sz; i3++)
      {
        send_int (((*pthis).a)[i3]);
      }
  }
  {
    int i4;
    int sz2 = 70;
    for (i4 = 0; i4 < sz2; i4++)
      {
        send_int (((*pthis).b)[i4]);
      }
  }
}

struct pentomino *
recv_pentomino_task ()
{
  struct pentomino *pthis = malloc (sizeof (struct pentomino));
  (*pthis).k = recv_int ();
  (*pthis).i0 = recv_int ();
  (*pthis).i1 = recv_int ();
  (*pthis).i2 = recv_int ();
  {
    int i5;
    int sz3 = 12;
    for (i5 = 0; i5 < sz3; i5++)
      {
        ((*pthis).a)[i5] = recv_int ();
      }
  }
  {
    int i6;
    int sz4 = 70;
    for (i6 = 0; i6 < sz4; i6++)
      {
        ((*pthis).b)[i6] = recv_int ();
      }
  }
  return pthis;
}

void
send_pentomino_rslt (struct pentomino *pthis)
{
  send_int ((*pthis).r);
  free (pthis);
}

void
recv_pentomino_rslt (struct pentomino *pthis)
{
  (*pthis).r = recv_int ();
}

void (*task_doers[256]) (struct thread_data *, void *) =
{
(void (*)(struct thread_data *, void *)) do_pentomino_task,
    (void (*)(struct thread_data *, void *)) do_start_pentomino_task};
void (*task_senders[256]) (void *) =
{
(void (*)(void *)) send_pentomino_task,
    (void (*)(void *)) send_start_pentomino_task};
void *(*task_receivers[256]) () =
{
(void *(*)()) recv_pentomino_task, (void *(*)()) recv_start_pentomino_task};
void (*rslt_senders[256]) (void *) =
{
(void (*)(void *)) send_pentomino_rslt,
    (void (*)(void *)) send_start_pentomino_rslt};
void (*rslt_receivers[256]) (void *) =
{
(void (*)(void *)) recv_pentomino_rslt,
    (void (*)(void *)) recv_start_pentomino_rslt};
