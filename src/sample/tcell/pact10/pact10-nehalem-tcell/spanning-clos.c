#include<stdio.h>
typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double Align_t;

char *lw_call (char *esp);

struct func_arg
{
  void *(*func) (char *, void *);
  void *arg;
};

void *thread_origin (void *farg);
struct do_t_search1_task_frame;
struct _bk_in_do_t_search1_task_frame;
struct search1_p_frame;
struct do_many_bk_in_search1_p_frame;
struct search_p_frame;
struct do_t_search_b_task_frame;
struct _bk_in_do_t_search_b_task_frame;
struct search_p_b_frame;
struct do_many_bk2_in_search_p_b_frame;
struct search_p_array_frame;
struct do_t_search_b_array_task_frame;
struct _bk_in_do_t_search_b_array_task_frame;
struct search_p_b_array_frame;
struct do_many_bk3_in_search_p_b_array_frame;
struct do_t_search_c_task_frame;
struct _bk_in_do_t_search_c_task_frame;
struct search_p_c_frame;
struct do_many_bk4_in_search_p_c_frame;
struct do_spanning_start_task_frame;
struct _bk_in_do_spanning_start_task_frame;

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
void (*task_doers[256]) (char *, struct thread_data *, void *);
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

void *wait_rslt (char *esp, struct thread_data *thr);

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

void handle_req (char *esp, closure_t *, struct thread_data *);

#include<sys/time.h>
#include<stdlib.h>
#include"xccmem.h"

int printf (char const *, ...);

int fprintf (FILE *, char const *, ...);

void *malloc (size_t);

void free (void *);

double sqrt (double);

double fabs (double);

double elapsed_time (struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0e-6 * ((tp[1]).tv_usec -
                                                     (tp[0]).tv_usec);
}

int V_Length;

struct vertex
{
  int degree;
  int first_e;
  int parent;
};
struct vertex VV[16 * 1024 * 1024 + 321];
int E_Length;

struct edge
{
  int next_v;
  int next_e;
};
struct edge EE[64 * 1024 * 1024 + 321];

struct v_list_body
{
  struct v_list_body *next;
  int i;
  int v[600];
};
int V_List_Length;
struct v_list_body *V_List;

struct v_list_head
{
  struct v_list_body *first;
  struct v_list_body *last;
  int length;
};

struct worker_data
{
  struct v_list_head V_List;
  char _dummy_[1000];
};

struct v_list_body *alloc_v_list (struct v_list_body *next);

void worker_init (struct thread_data *_thr)
{
  (*((struct worker_data *) (*_thr).wdptr)).V_List.first = 0;
  (*((struct worker_data *) (*_thr).wdptr)).V_List.last = 0;
  (*((struct worker_data *) (*_thr).wdptr)).V_List.length = 0;
}

struct v_list_body *alloc_v_list (struct v_list_body *next)
{
  struct v_list_body *a;
  a = (struct v_list_body *) malloc (sizeof (struct v_list_body));
  (*a).next = next;
  (*a).i = 600;
  return a;
}

struct v_list_body *dealloc_v_list (struct v_list_body *a)
{
  struct v_list_body *next;

  next = (*a).next;
  free (a);
  return next;
}

void check_last (struct v_list_body *v_list, struct v_list_body *v_list_last,
                 char *info)
{
  struct v_list_body *cur;

  if (!v_list)
    return;
  else;
  {
    cur = v_list;
    for (; (*cur).next; cur = (*cur).next)
      {
      }
  }
  if (cur != v_list_last)
    {
      fprintf (stderr, "!! last is incorrect. %s \n", info);
    }
  else;
}

void check_length (struct v_list_body *v_list, int len_expect, char *info)
{
  struct v_list_body *cur;
  int len;

  len = 0;
  {
    cur = v_list;
    for (; cur; cur = (*cur).next)
      {
        len++;
      }
  }
  if (len != len_expect)
    {
      fprintf (stderr, "!! Unexpected length len:%d expect:%d %s\n", len,
               len_expect, info);
    }
  else;
}

void check_v_list (struct v_list_body *v_list, struct v_list_body *v_list_last,
                   int len_expect, char *info)
{
  check_length (v_list, len_expect, info);
  check_last (v_list, v_list_last, info);
}

struct v_list_body *nconc_v_list (struct v_list_body *a,
                                  struct v_list_body *a_last,
                                  struct v_list_body *b)
{
  struct v_list_body *cur;

  if (a_last)
    {
      (*a_last).next = b;
      return a;
    }
  else if (a)
    {
      {
        cur = a;
        for (; (*cur).next; cur = (*cur).next)
          {
          }
      }
      (*cur).next = b;
      return a;
    }
  else
    return b;
}

void v_list_to_array (struct v_list_body **buf,
                      struct v_list_body *v_list_first, int len)
{
  int i;
  struct v_list_body *v_list_cur;

  {
    i = 0, v_list_cur = v_list_first;
    for (; i < len; (i++, v_list_cur = (*v_list_cur).next))
      {
        buf[i] = v_list_cur;
      }
  }
}

void search_s2 (int fv)
{
  struct v_list_body *my_v_list;
  int my_v_list_length;
  int vi;
  int v;
  int i;
  int d;
  int e;
  int nv;

  (VV[fv]).parent = fv + 1;
  my_v_list = alloc_v_list (0);
  my_v_list_length = 1;
  vi = (*my_v_list).i;
  ((*my_v_list).v)[--vi] = fv;
  while (vi < 600)
    {
      v = ((*my_v_list).v)[vi++];
      d = (VV[v]).degree;
      e = (VV[v]).first_e;
      {
        i = 0;
        for (; i < d; (i++, e = (EE[e]).next_e))
          {
            nv = (EE[e]).next_v;
            if ((VV[nv]).parent == 0)
              if (1)
                {
                  (VV[nv]).parent = v + 1;
                  if (vi <= 0)
                    {
                      (*my_v_list).i = vi;
                      my_v_list = alloc_v_list (my_v_list);
                      my_v_list_length++;
                      vi = (*my_v_list).i;
                    }
                  else;
                  ((*my_v_list).v)[--vi] = nv;
                }
              else;
            else;
          }
      }
      while (vi == 600 && my_v_list_length > 1)
        {
          my_v_list = dealloc_v_list (my_v_list);
          my_v_list_length--;
          vi = (*my_v_list).i;
        }
    }
  (*my_v_list).i = vi;
}

void search1 (int v, int b)
{
  int i;
  int d;
  int e;
  int nv;

  d = (VV[v]).degree;
  e = (VV[v]).first_e;
  {
    i = 0;
    for (; i < d; (i++, e = (EE[e]).next_e))
      {
        nv = (EE[e]).next_v;
        if ((VV[nv]).parent == 0)
          if (1)
            {
              (VV[nv]).parent = v + 1;
              if (b > 0)
                search1 (nv, b - 1);
              else
                {
                  if ((*V_List).i <= 0)
                    {
                      V_List = alloc_v_list (V_List);
                      V_List_Length++;
                    }
                  else;
                  ((*V_List).v)[--(*V_List).i] = nv;
                }
            }
          else;
        else;
      }
  }
}

void search (int v)
{
  struct v_list_body *my_v_list;
  struct v_list_body *mlist;
  struct v_list_body *next;
  int i;
  int j;
  int my_v_list_length;
  int mlist_length;

  {
  }
  (VV[v]).parent = v + 1;
  V_List = alloc_v_list (0);
  V_List_Length = 1;
  search1 (v, 800);
  {
  }
  do
    {
      my_v_list = V_List;
      mlist = my_v_list;
      my_v_list_length = V_List_Length;
      mlist_length = my_v_list_length;
      V_List = alloc_v_list (0);
      V_List_Length = 1;
      {
        i = 0;
        for (; i < my_v_list_length; (i++, my_v_list = (*my_v_list).next))
          {
            j = (*my_v_list).i;
            for (; j < 600; j++)
              {
                search1 (((*my_v_list).v)[j], 800);
              }
          }
      }
      {
        i = 0;
        for (; i < mlist_length; i++)
          {
            mlist = dealloc_v_list (mlist);
          }
      }
      {
      }
    }
  while (V_List_Length > 1 || (*V_List).i < 600);
}

void search1_p (char *esp, closure_t * _bk, struct thread_data *_thr, int v,
                int b, int k0, int k1);

void search_p_b (char *esp, closure_t * _bk, struct thread_data *_thr,
                 struct v_list_body *my_v_list, int i0, int i1);

void search_p_b_array (char *esp, closure_t * _bk, struct thread_data *_thr,
                       struct v_list_body **my_v_array, int i0, int i1);

void search_p_c (char *esp, closure_t * _bk, struct thread_data *_thr,
                 struct v_list_body *my_v_list, int j0, int j1);

struct t_search1
{
  int v;
  int b;
  int k0;
  int k1;
  struct v_list_head v_list;
  char _dummy_[1000];
};

struct _bk_in_do_t_search1_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_t_search1_task_frame *xfp;
};

struct do_t_search1_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int stack_created_p;
  int ifexp_result;
  int ifexp_result2;
  struct t_search1 *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

char *_bk_in_do_t_search1_task (char *esp, void *xfp0)
{
  struct _bk_in_do_t_search1_task_frame *efp;
  struct do_t_search1_task_frame *xfp = xfp0;
  char *parmp = (char *) esp;

LGOTO:;
  efp = (struct _bk_in_do_t_search1_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct _bk_in_do_t_search1_task_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void do_t_search1_task (char *esp, struct thread_data *_thr,
                        struct t_search1 *pthis)
{
  struct do_t_search1_task_frame *efp;

LGOTO:;
  efp = (struct do_t_search1_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_t_search1_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->_thr = _thr;
  efp->pthis = pthis;
  efp->_bk.fun = _bk_in_do_t_search1_task;
  efp->_bk.fr = (void *) efp;
  efp->stack_created_p = 0;
  if (!(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first)
    {
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first =
        alloc_v_list (0);
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length = 1;
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last =
        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first;
      efp->stack_created_p = 1;
    }
  else;
  while (search1_p
         (esp, &efp->_bk, efp->_thr, (*efp->pthis).v, (*efp->pthis).b,
          (*efp->pthis).k0, (*efp->pthis).k1), __builtin_expect ((efp->tmp_esp =
                                                                  *((char **)
                                                                    esp)) != 0,
                                                                 0))
    {
      if ((char *) efp->tmp_esp < (char *) efp)
        return;
      else;
      efp->tmp_esp = 0;
      goto LGOTO;
    }
  if (efp->stack_created_p)
    {
      {
        if ((*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length > 1)
          efp->ifexp_result = 1;
        else
          {
            if ((*(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first).
                i < 600)
              efp->ifexp_result2 = 1;
            else
              efp->ifexp_result2 = 0;
            efp->ifexp_result = efp->ifexp_result2;
          }
        if (efp->ifexp_result)
          {
            (*efp->pthis).v_list.first =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first;
            (*efp->pthis).v_list.last =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last;
            (*efp->pthis).v_list.length =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length;
          }
        else
          {
            dealloc_v_list ((*((struct worker_data *) (*efp->_thr).wdptr)).
                            V_List.first);
            (*efp->pthis).v_list.first = 0;
            (*efp->pthis).v_list.last = 0;
            (*efp->pthis).v_list.length = 0;
          }
      }
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first = 0;
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last = 0;
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length = 0;
    }
  else
    {
      (*efp->pthis).v_list.first = 0;
      (*efp->pthis).v_list.last = 0;
      (*efp->pthis).v_list.length = 0;
    }
}

struct do_many_bk_in_search1_p_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int k0_2;
  int k1_2;
  int ifexp_result5;
  int ifexp_result6;
  int ifexp_result3;
  int ifexp_result4;
  struct search1_p_frame *xfp;
};

struct search1_p_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int spawned;
  struct t_search1 *pthis;
  int I_end;
  int i2;
  int nv;
  int e;
  int i;
  void *tmp;
  int k1;
  int k0;
  int b;
  int v;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t do_many_bk;
};

char *do_many_bk_in_search1_p (char *esp, void *xfp0)
{
  struct do_many_bk_in_search1_p_frame *efp;
  struct search1_p_frame *xfp = xfp0;
  char *parmp = (char *) esp;
  char *argp;

LGOTO:;
  efp = (struct do_many_bk_in_search1_p_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_many_bk_in_search1_p_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  if (!xfp->spawned)
    {
      char *goto_fr;
      argp =
        (char *) ((Align_t *) esp +
                  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
      *((closure_t **) argp) = xfp->_bk;
      goto_fr = lw_call (argp);
      if (goto_fr)
        if ((char *) goto_fr < (char *) efp)
          return goto_fr;
        else
          {
            efp->tmp_esp = 0;
            goto LGOTO;
          }
      else;
    }
  else;
  {
    if ((*xfp->_thr).treq_top)
      {
        if (xfp->i2 + 1 < xfp->I_end)
          efp->ifexp_result4 = 1;
        else
          efp->ifexp_result4 = 0;
        efp->ifexp_result3 = efp->ifexp_result4;
      }
    else
      efp->ifexp_result3 = 0;
    if (efp->ifexp_result3)
      {
        goto loop_start;
        while (1)
          {
            if ((*xfp->_thr).treq_top)
              {
                if (xfp->i2 + 1 < xfp->I_end)
                  efp->ifexp_result6 = 1;
                else
                  efp->ifexp_result6 = 0;
                efp->ifexp_result5 = efp->ifexp_result6;
              }
            else
              efp->ifexp_result5 = 0;
            if (!efp->ifexp_result5)
              goto loop_end;
            else;
          loop_start:;
            efp->k1_2 = xfp->I_end;
            efp->k0_2 = (1 + xfp->i2 + xfp->I_end) / 2;
            xfp->I_end = efp->k0_2;
            xfp->pthis =
              (struct t_search1 *) malloc (sizeof (struct t_search1));
            (*xfp->pthis).v = xfp->v;
            (*xfp->pthis).b = xfp->b;
            (*xfp->pthis).k0 = efp->k0_2;
            (*xfp->pthis).k1 = efp->k1_2;
            (xfp->spawned)++;
            make_and_send_task (xfp->_thr, 0, xfp->pthis);
          }
      loop_end:;
      }
    else;
  }
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void search1_p (char *esp, closure_t * _bk, struct thread_data *_thr, int v,
                int b, int k0, int k1)
{
  struct search1_p_frame *efp;

LGOTO:;
  efp = (struct search1_p_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct search1_p_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->_bk = _bk;
  efp->_thr = _thr;
  efp->v = v;
  efp->b = b;
  efp->k0 = k0;
  efp->k1 = k1;
  efp->do_many_bk.fun = do_many_bk_in_search1_p;
  efp->do_many_bk.fr = (void *) efp;
  efp->e = (VV[efp->v]).first_e;
  {
    efp->i = 0;
    for (; efp->i < efp->k0; (efp->i)++)
      {
        efp->e = (EE[efp->e]).next_e;
      }
  }
  {
    efp->i2 = efp->k0;
    efp->I_end = efp->k1;
    efp->spawned = 0;
    if ((*efp->_thr).req)
      while (handle_req (esp, &efp->do_many_bk, efp->_thr),
             __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
        {
          if ((char *) efp->tmp_esp < (char *) efp)
            return;
          else;
          efp->tmp_esp = 0;
          goto LGOTO;
        }
    else;
    for (; efp->i2 < efp->I_end; (efp->i2)++)
      {
        efp->nv = (EE[efp->e]).next_v;
        if (!cas_int ((VV[efp->nv]).parent, 0, efp->v + 1))
          {
            start_access_after_lock ();
            if (efp->b > 0)
              while (search1_p
                     (esp, &efp->do_many_bk, efp->_thr, efp->nv, efp->b - 1, 0,
                      (VV[efp->nv]).degree), __builtin_expect ((efp->tmp_esp =
                                                                *((char **)
                                                                  esp)) != 0,
                                                               0))
                {
                  if ((char *) efp->tmp_esp < (char *) efp)
                    return;
                  else;
                  efp->tmp_esp = 0;
                  goto LGOTO;
                }
            else
              {
                if ((*(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.
                     first).i <= 0)
                  {
                    (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.
                      first =
                      alloc_v_list ((*
                                     ((struct worker_data *) (*efp->_thr).
                                      wdptr)).V_List.first);
                    ((*((struct worker_data *) (*efp->_thr).wdptr)).V_List.
                     length)++;
                  }
                else;
                ((*(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first).
                 v)[--(*(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.
                       first).i] = efp->nv;
              }
          }
        else;
        efp->e = (EE[efp->e]).next_e;
      }
    while ((efp->spawned)-- > 0)
      {
        {
          while (__builtin_expect
                 ((efp->tmp = wait_rslt (esp, efp->_thr)) == (void *) 0 - 1, 0)
                 && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                      1))
            {
              if ((char *) efp->tmp_esp < (char *) efp)
                return;
              else;
              efp->tmp_esp = 0;
              goto LGOTO;
            }
          efp->pthis = (struct t_search1 *) efp->tmp;
        }
        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first =
          nconc_v_list ((*efp->pthis).v_list.first, (*efp->pthis).v_list.last,
                        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.
                        first);
        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length +=
          (*efp->pthis).v_list.length;
        free (efp->pthis);
      }
  }
}

struct search_p_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i;
  struct v_list_head mlist;
  struct v_list_head my_v_list;
  int ifexp_result9;
  int ifexp_result10;
  int ifexp_result7;
  int ifexp_result8;
  int v;
  struct thread_data *_thr;
  closure_t *_bk;
};

void search_p (char *esp, closure_t * _bk, struct thread_data *_thr, int v)
{
  struct search_p_frame *efp;

LGOTO:;
  efp = (struct search_p_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct search_p_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->_bk = _bk;
  efp->_thr = _thr;
  efp->v = v;
  {
  }
  (VV[efp->v]).parent = efp->v + 1;
  (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first =
    alloc_v_list (0);
  (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length = 1;
  (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last =
    (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first;
  while (search1_p
         (esp, efp->_bk, efp->_thr, efp->v, 800, 0, (VV[efp->v]).degree),
         __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
    {
      if ((char *) efp->tmp_esp < (char *) efp)
        return;
      else;
      efp->tmp_esp = 0;
      goto LGOTO;
    }
  {
  }
  {
    if ((*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length > 1)
      efp->ifexp_result7 = 1;
    else
      {
        if ((*(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first).i <
            600)
          efp->ifexp_result8 = 1;
        else
          efp->ifexp_result8 = 0;
        efp->ifexp_result7 = efp->ifexp_result8;
      }
    if (efp->ifexp_result7)
      {
        goto loop_start2;
        while (1)
          {
            if ((*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length >
                1)
              efp->ifexp_result9 = 1;
            else
              {
                if ((*(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.
                     first).i < 600)
                  efp->ifexp_result10 = 1;
                else
                  efp->ifexp_result10 = 0;
                efp->ifexp_result9 = efp->ifexp_result10;
              }
            if (!efp->ifexp_result9)
              goto loop_end2;
            else;
          loop_start2:;
            efp->my_v_list =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List;
            efp->mlist = efp->my_v_list;
            (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first =
              alloc_v_list (0);
            (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length = 1;
            (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first;
            {
            }
            while (search_p_b
                   (esp, efp->_bk, efp->_thr, (efp->my_v_list).first, 0,
                    (efp->my_v_list).length), __builtin_expect ((efp->tmp_esp =
                                                                 *((char **)
                                                                   esp)) != 0,
                                                                0))
              {
                if ((char *) efp->tmp_esp < (char *) efp)
                  return;
                else;
                efp->tmp_esp = 0;
                goto LGOTO;
              }
            {
              efp->i = 0;
              for (; efp->i < (efp->mlist).length; (efp->i)++)
                {
                  (efp->mlist).first = dealloc_v_list ((efp->mlist).first);
                }
            }
          }
      loop_end2:;
      }
    else;
  }
  {
  }
}

struct t_search_b
{
  struct v_list_body *my_v_list_first;
  int i0;
  int i1;
  struct v_list_head v_list;
  char _dummy_[1000];
};

struct _bk_in_do_t_search_b_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_t_search_b_task_frame *xfp;
};

struct do_t_search_b_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int stack_created_p;
  int ifexp_result11;
  int ifexp_result12;
  struct t_search_b *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

char *_bk_in_do_t_search_b_task (char *esp, void *xfp0)
{
  struct _bk_in_do_t_search_b_task_frame *efp;
  struct do_t_search_b_task_frame *xfp = xfp0;
  char *parmp = (char *) esp;

LGOTO:;
  efp = (struct _bk_in_do_t_search_b_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct _bk_in_do_t_search_b_task_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void do_t_search_b_task (char *esp, struct thread_data *_thr,
                         struct t_search_b *pthis)
{
  struct do_t_search_b_task_frame *efp;

LGOTO:;
  efp = (struct do_t_search_b_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_t_search_b_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->_thr = _thr;
  efp->pthis = pthis;
  efp->_bk.fun = _bk_in_do_t_search_b_task;
  efp->_bk.fr = (void *) efp;
  efp->stack_created_p = 0;
  if (!(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first)
    {
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first =
        alloc_v_list (0);
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length = 1;
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last =
        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first;
      efp->stack_created_p = 1;
    }
  else;
  while (search_p_b
         (esp, &efp->_bk, efp->_thr, (*efp->pthis).my_v_list_first,
          (*efp->pthis).i0, (*efp->pthis).i1), __builtin_expect ((efp->tmp_esp =
                                                                  *((char **)
                                                                    esp)) != 0,
                                                                 0))
    {
      if ((char *) efp->tmp_esp < (char *) efp)
        return;
      else;
      efp->tmp_esp = 0;
      goto LGOTO;
    }
  if (efp->stack_created_p)
    {
      {
        if ((*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length > 1)
          efp->ifexp_result11 = 1;
        else
          {
            if ((*(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first).
                i < 600)
              efp->ifexp_result12 = 1;
            else
              efp->ifexp_result12 = 0;
            efp->ifexp_result11 = efp->ifexp_result12;
          }
        if (efp->ifexp_result11)
          {
            (*efp->pthis).v_list.first =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first;
            (*efp->pthis).v_list.last =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last;
            (*efp->pthis).v_list.length =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length;
          }
        else
          {
            dealloc_v_list ((*((struct worker_data *) (*efp->_thr).wdptr)).
                            V_List.first);
            (*efp->pthis).v_list.first = 0;
            (*efp->pthis).v_list.last = 0;
            (*efp->pthis).v_list.length = 0;
          }
      }
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first = 0;
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last = 0;
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length = 0;
    }
  else
    {
      (*efp->pthis).v_list.first = 0;
      (*efp->pthis).v_list.last = 0;
      (*efp->pthis).v_list.length = 0;
    }
}

struct do_many_bk2_in_search_p_b_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i0_2;
  int i1_2;
  int ifexp_result15;
  int ifexp_result16;
  int ifexp_result13;
  int ifexp_result14;
  struct search_p_b_frame *xfp;
};

struct search_p_b_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int spawned2;
  struct t_search_b *pthis;
  int I_end2;
  int i3;
  struct v_list_body *my_v_list_first0;
  int i;
  void *tmp2;
  int i1;
  int i0;
  struct v_list_body *my_v_list_first;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t do_many_bk2;
};

char *do_many_bk2_in_search_p_b (char *esp, void *xfp0)
{
  struct do_many_bk2_in_search_p_b_frame *efp;
  struct search_p_b_frame *xfp = xfp0;
  char *parmp = (char *) esp;
  char *argp;

LGOTO:;
  efp = (struct do_many_bk2_in_search_p_b_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_many_bk2_in_search_p_b_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  if (!xfp->spawned2)
    {
      char *goto_fr;
      argp =
        (char *) ((Align_t *) esp +
                  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
      *((closure_t **) argp) = xfp->_bk;
      goto_fr = lw_call (argp);
      if (goto_fr)
        if ((char *) goto_fr < (char *) efp)
          return goto_fr;
        else
          {
            efp->tmp_esp = 0;
            goto LGOTO;
          }
      else;
    }
  else;
  {
    if ((*xfp->_thr).treq_top)
      {
        if (xfp->i3 + 1 < xfp->I_end2)
          efp->ifexp_result14 = 1;
        else
          efp->ifexp_result14 = 0;
        efp->ifexp_result13 = efp->ifexp_result14;
      }
    else
      efp->ifexp_result13 = 0;
    if (efp->ifexp_result13)
      {
        goto loop_start3;
        while (1)
          {
            if ((*xfp->_thr).treq_top)
              {
                if (xfp->i3 + 1 < xfp->I_end2)
                  efp->ifexp_result16 = 1;
                else
                  efp->ifexp_result16 = 0;
                efp->ifexp_result15 = efp->ifexp_result16;
              }
            else
              efp->ifexp_result15 = 0;
            if (!efp->ifexp_result15)
              goto loop_end3;
            else;
          loop_start3:;
            efp->i1_2 = xfp->I_end2;
            efp->i0_2 = (1 + xfp->i3 + xfp->I_end2) / 2;
            xfp->I_end2 = efp->i0_2;
            xfp->pthis =
              (struct t_search_b *) malloc (sizeof (struct t_search_b));
            (*xfp->pthis).my_v_list_first = xfp->my_v_list_first0;
            (*xfp->pthis).i0 = efp->i0_2;
            (*xfp->pthis).i1 = efp->i1_2;
            (xfp->spawned2)++;
            make_and_send_task (xfp->_thr, 1, xfp->pthis);
          }
      loop_end3:;
      }
    else;
  }
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void search_p_b (char *esp, closure_t * _bk, struct thread_data *_thr,
                 struct v_list_body *my_v_list_first, int i0, int i1)
{
  struct search_p_b_frame *efp;

LGOTO:;
  efp = (struct search_p_b_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct search_p_b_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->_bk = _bk;
  efp->_thr = _thr;
  efp->my_v_list_first = my_v_list_first;
  efp->i0 = i0;
  efp->i1 = i1;
  efp->do_many_bk2.fun = do_many_bk2_in_search_p_b;
  efp->do_many_bk2.fr = (void *) efp;
  {
  }
  {
    efp->i = 0;
    for (; efp->i < efp->i0; (efp->i)++)
      {
        efp->my_v_list_first = (*efp->my_v_list_first).next;
      }
  }
  efp->i1 -= efp->i0;
  efp->i0 = 0;
  efp->my_v_list_first0 = efp->my_v_list_first;
  {
    efp->i3 = efp->i0;
    efp->I_end2 = efp->i1;
    efp->spawned2 = 0;
    if ((*efp->_thr).req)
      while (handle_req (esp, &efp->do_many_bk2, efp->_thr),
             __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
        {
          if ((char *) efp->tmp_esp < (char *) efp)
            return;
          else;
          efp->tmp_esp = 0;
          goto LGOTO;
        }
    else;
    for (; efp->i3 < efp->I_end2; (efp->i3)++)
      {
        while (search_p_c
               (esp, &efp->do_many_bk2, efp->_thr, efp->my_v_list_first,
                (*efp->my_v_list_first).i, 600),
               __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
          {
            if ((char *) efp->tmp_esp < (char *) efp)
              return;
            else;
            efp->tmp_esp = 0;
            goto LGOTO;
          }
        efp->my_v_list_first = (*efp->my_v_list_first).next;
      }
    while ((efp->spawned2)-- > 0)
      {
        {
          while (__builtin_expect
                 ((efp->tmp2 = wait_rslt (esp, efp->_thr)) == (void *) 0 - 1, 0)
                 && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                      1))
            {
              if ((char *) efp->tmp_esp < (char *) efp)
                return;
              else;
              efp->tmp_esp = 0;
              goto LGOTO;
            }
          efp->pthis = (struct t_search_b *) efp->tmp2;
        }
        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first =
          nconc_v_list ((*efp->pthis).v_list.first, (*efp->pthis).v_list.last,
                        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.
                        first);
        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length +=
          (*efp->pthis).v_list.length;
        free (efp->pthis);
      }
  }
}

struct search_p_array_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i;
  int buf_size;
  int my_v_array_length;
  struct v_list_body **my_v_array;
  int ifexp_result19;
  int ifexp_result20;
  int ifexp_result17;
  int ifexp_result18;
  int v;
  struct thread_data *_thr;
  closure_t *_bk;
};

void search_p_array (char *esp, closure_t * _bk, struct thread_data *_thr,
                     int v)
{
  struct search_p_array_frame *efp;

LGOTO:;
  efp = (struct search_p_array_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct search_p_array_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->_bk = _bk;
  efp->_thr = _thr;
  efp->v = v;
  {
  }
  (VV[efp->v]).parent = efp->v + 1;
  (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first =
    alloc_v_list (0);
  (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length = 1;
  (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last =
    (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first;
  while (search1_p
         (esp, efp->_bk, efp->_thr, efp->v, 800, 0, (VV[efp->v]).degree),
         __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
    {
      if ((char *) efp->tmp_esp < (char *) efp)
        return;
      else;
      efp->tmp_esp = 0;
      goto LGOTO;
    }
  {
  }
  efp->my_v_array = 0;
  efp->buf_size = 0;
  {
    if ((*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length > 1)
      efp->ifexp_result17 = 1;
    else
      {
        if ((*(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first).i <
            600)
          efp->ifexp_result18 = 1;
        else
          efp->ifexp_result18 = 0;
        efp->ifexp_result17 = efp->ifexp_result18;
      }
    if (efp->ifexp_result17)
      {
        goto loop_start4;
        while (1)
          {
            if ((*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length >
                1)
              efp->ifexp_result19 = 1;
            else
              {
                if ((*(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.
                     first).i < 600)
                  efp->ifexp_result20 = 1;
                else
                  efp->ifexp_result20 = 0;
                efp->ifexp_result19 = efp->ifexp_result20;
              }
            if (!efp->ifexp_result19)
              goto loop_end4;
            else;
          loop_start4:;
            efp->my_v_array_length =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length;
            if (efp->my_v_array_length > efp->buf_size)
              {
                efp->buf_size = 2 * efp->my_v_array_length;
                efp->my_v_array =
                  (struct v_list_body **) realloc (efp->my_v_array,
                                                   efp->buf_size *
                                                   sizeof (struct v_list_body
                                                           *));
              }
            else;
            v_list_to_array (efp->my_v_array,
                             (*((struct worker_data *) (*efp->_thr).wdptr)).
                             V_List.first,
                             (*((struct worker_data *) (*efp->_thr).wdptr)).
                             V_List.length);
            (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first =
              alloc_v_list (0);
            (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length = 1;
            (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first;
            while (search_p_b_array
                   (esp, efp->_bk, efp->_thr, efp->my_v_array, 0,
                    efp->my_v_array_length), __builtin_expect ((efp->tmp_esp =
                                                                *((char **)
                                                                  esp)) != 0,
                                                               0))
              {
                if ((char *) efp->tmp_esp < (char *) efp)
                  return;
                else;
                efp->tmp_esp = 0;
                goto LGOTO;
              }
            {
              efp->i = 0;
              for (; efp->i < efp->my_v_array_length; (efp->i)++)
                {
                  dealloc_v_list ((efp->my_v_array)[efp->i]);
                }
            }
          }
      loop_end4:;
      }
    else;
  }
  free (efp->my_v_array);
  {
  }
}

struct t_search_b_array
{
  struct v_list_body **my_v_array;
  int i0;
  int i1;
  struct v_list_head v_list;
  char _dummy_[1000];
};

struct _bk_in_do_t_search_b_array_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_t_search_b_array_task_frame *xfp;
};

struct do_t_search_b_array_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int stack_created_p;
  int ifexp_result21;
  int ifexp_result22;
  struct t_search_b_array *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

char *_bk_in_do_t_search_b_array_task (char *esp, void *xfp0)
{
  struct _bk_in_do_t_search_b_array_task_frame *efp;
  struct do_t_search_b_array_task_frame *xfp = xfp0;
  char *parmp = (char *) esp;

LGOTO:;
  efp = (struct _bk_in_do_t_search_b_array_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct _bk_in_do_t_search_b_array_task_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void do_t_search_b_array_task (char *esp, struct thread_data *_thr,
                               struct t_search_b_array *pthis)
{
  struct do_t_search_b_array_task_frame *efp;

LGOTO:;
  efp = (struct do_t_search_b_array_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_t_search_b_array_task_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->_thr = _thr;
  efp->pthis = pthis;
  efp->_bk.fun = _bk_in_do_t_search_b_array_task;
  efp->_bk.fr = (void *) efp;
  efp->stack_created_p = 0;
  if (!(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first)
    {
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first =
        alloc_v_list (0);
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length = 1;
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last =
        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first;
      efp->stack_created_p = 1;
    }
  else;
  while (search_p_b_array
         (esp, &efp->_bk, efp->_thr, (*efp->pthis).my_v_array, (*efp->pthis).i0,
          (*efp->pthis).i1), __builtin_expect ((efp->tmp_esp =
                                                *((char **) esp)) != 0, 0))
    {
      if ((char *) efp->tmp_esp < (char *) efp)
        return;
      else;
      efp->tmp_esp = 0;
      goto LGOTO;
    }
  if (efp->stack_created_p)
    {
      {
        if ((*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length > 1)
          efp->ifexp_result21 = 1;
        else
          {
            if ((*(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first).
                i < 600)
              efp->ifexp_result22 = 1;
            else
              efp->ifexp_result22 = 0;
            efp->ifexp_result21 = efp->ifexp_result22;
          }
        if (efp->ifexp_result21)
          {
            (*efp->pthis).v_list.first =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first;
            (*efp->pthis).v_list.last =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last;
            (*efp->pthis).v_list.length =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length;
          }
        else
          {
            dealloc_v_list ((*((struct worker_data *) (*efp->_thr).wdptr)).
                            V_List.first);
            (*efp->pthis).v_list.first = 0;
            (*efp->pthis).v_list.last = 0;
            (*efp->pthis).v_list.length = 0;
          }
      }
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first = 0;
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last = 0;
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length = 0;
    }
  else
    {
      (*efp->pthis).v_list.first = 0;
      (*efp->pthis).v_list.last = 0;
      (*efp->pthis).v_list.length = 0;
    }
}

struct do_many_bk3_in_search_p_b_array_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int i0_2;
  int i1_2;
  int ifexp_result25;
  int ifexp_result26;
  int ifexp_result23;
  int ifexp_result24;
  struct search_p_b_array_frame *xfp;
};

struct search_p_b_array_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int spawned3;
  struct t_search_b_array *pthis;
  int I_end3;
  int i4;
  int i;
  void *tmp3;
  int i1;
  int i0;
  struct v_list_body **my_v_array;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t do_many_bk3;
};

char *do_many_bk3_in_search_p_b_array (char *esp, void *xfp0)
{
  struct do_many_bk3_in_search_p_b_array_frame *efp;
  struct search_p_b_array_frame *xfp = xfp0;
  char *parmp = (char *) esp;
  char *argp;

LGOTO:;
  efp = (struct do_many_bk3_in_search_p_b_array_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_many_bk3_in_search_p_b_array_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  if (!xfp->spawned3)
    {
      char *goto_fr;
      argp =
        (char *) ((Align_t *) esp +
                  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
      *((closure_t **) argp) = xfp->_bk;
      goto_fr = lw_call (argp);
      if (goto_fr)
        if ((char *) goto_fr < (char *) efp)
          return goto_fr;
        else
          {
            efp->tmp_esp = 0;
            goto LGOTO;
          }
      else;
    }
  else;
  {
    if ((*xfp->_thr).treq_top)
      {
        if (xfp->i4 + 1 < xfp->I_end3)
          efp->ifexp_result24 = 1;
        else
          efp->ifexp_result24 = 0;
        efp->ifexp_result23 = efp->ifexp_result24;
      }
    else
      efp->ifexp_result23 = 0;
    if (efp->ifexp_result23)
      {
        goto loop_start5;
        while (1)
          {
            if ((*xfp->_thr).treq_top)
              {
                if (xfp->i4 + 1 < xfp->I_end3)
                  efp->ifexp_result26 = 1;
                else
                  efp->ifexp_result26 = 0;
                efp->ifexp_result25 = efp->ifexp_result26;
              }
            else
              efp->ifexp_result25 = 0;
            if (!efp->ifexp_result25)
              goto loop_end5;
            else;
          loop_start5:;
            efp->i1_2 = xfp->I_end3;
            efp->i0_2 = (1 + xfp->i4 + xfp->I_end3) / 2;
            xfp->I_end3 = efp->i0_2;
            xfp->pthis =
              (struct t_search_b_array *)
              malloc (sizeof (struct t_search_b_array));
            (*xfp->pthis).my_v_array = xfp->my_v_array;
            (*xfp->pthis).i0 = efp->i0_2;
            (*xfp->pthis).i1 = efp->i1_2;
            (xfp->spawned3)++;
            make_and_send_task (xfp->_thr, 2, xfp->pthis);
          }
      loop_end5:;
      }
    else;
  }
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void search_p_b_array (char *esp, closure_t * _bk, struct thread_data *_thr,
                       struct v_list_body **my_v_array, int i0, int i1)
{
  struct search_p_b_array_frame *efp;

LGOTO:;
  efp = (struct search_p_b_array_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct search_p_b_array_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->_bk = _bk;
  efp->_thr = _thr;
  efp->my_v_array = my_v_array;
  efp->i0 = i0;
  efp->i1 = i1;
  efp->do_many_bk3.fun = do_many_bk3_in_search_p_b_array;
  efp->do_many_bk3.fr = (void *) efp;
  {
  }
  {
    efp->i4 = efp->i0;
    efp->I_end3 = efp->i1;
    efp->spawned3 = 0;
    if ((*efp->_thr).req)
      while (handle_req (esp, &efp->do_many_bk3, efp->_thr),
             __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
        {
          if ((char *) efp->tmp_esp < (char *) efp)
            return;
          else;
          efp->tmp_esp = 0;
          goto LGOTO;
        }
    else;
    for (; efp->i4 < efp->I_end3; (efp->i4)++)
      {
        while (search_p_c
               (esp, &efp->do_many_bk3, efp->_thr, (efp->my_v_array)[efp->i4],
                (*(efp->my_v_array)[efp->i4]).i, 600),
               __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
          {
            if ((char *) efp->tmp_esp < (char *) efp)
              return;
            else;
            efp->tmp_esp = 0;
            goto LGOTO;
          }
      }
    while ((efp->spawned3)-- > 0)
      {
        {
          while (__builtin_expect
                 ((efp->tmp3 = wait_rslt (esp, efp->_thr)) == (void *) 0 - 1, 0)
                 && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                      1))
            {
              if ((char *) efp->tmp_esp < (char *) efp)
                return;
              else;
              efp->tmp_esp = 0;
              goto LGOTO;
            }
          efp->pthis = (struct t_search_b_array *) efp->tmp3;
        }
        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first =
          nconc_v_list ((*efp->pthis).v_list.first, (*efp->pthis).v_list.last,
                        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.
                        first);
        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length +=
          (*efp->pthis).v_list.length;
        free (efp->pthis);
      }
  }
}

struct t_search_c
{
  struct v_list_body *my_v_list_cur;
  int j0;
  int j1;
  struct v_list_head v_list;
  char _dummy_[1000];
};

struct _bk_in_do_t_search_c_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_t_search_c_task_frame *xfp;
};

struct do_t_search_c_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int stack_created_p;
  int ifexp_result27;
  int ifexp_result28;
  struct t_search_c *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

char *_bk_in_do_t_search_c_task (char *esp, void *xfp0)
{
  struct _bk_in_do_t_search_c_task_frame *efp;
  struct do_t_search_c_task_frame *xfp = xfp0;
  char *parmp = (char *) esp;

LGOTO:;
  efp = (struct _bk_in_do_t_search_c_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct _bk_in_do_t_search_c_task_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void do_t_search_c_task (char *esp, struct thread_data *_thr,
                         struct t_search_c *pthis)
{
  struct do_t_search_c_task_frame *efp;

LGOTO:;
  efp = (struct do_t_search_c_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_t_search_c_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->_thr = _thr;
  efp->pthis = pthis;
  efp->_bk.fun = _bk_in_do_t_search_c_task;
  efp->_bk.fr = (void *) efp;
  efp->stack_created_p = 0;
  if (!(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first)
    {
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first =
        alloc_v_list (0);
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length = 1;
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last =
        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first;
      efp->stack_created_p = 1;
    }
  else;
  while (search_p_c
         (esp, &efp->_bk, efp->_thr, (*efp->pthis).my_v_list_cur,
          (*efp->pthis).j0, (*efp->pthis).j1), __builtin_expect ((efp->tmp_esp =
                                                                  *((char **)
                                                                    esp)) != 0,
                                                                 0))
    {
      if ((char *) efp->tmp_esp < (char *) efp)
        return;
      else;
      efp->tmp_esp = 0;
      goto LGOTO;
    }
  if (efp->stack_created_p)
    {
      {
        if ((*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length > 1)
          efp->ifexp_result27 = 1;
        else
          {
            if ((*(*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first).
                i < 600)
              efp->ifexp_result28 = 1;
            else
              efp->ifexp_result28 = 0;
            efp->ifexp_result27 = efp->ifexp_result28;
          }
        if (efp->ifexp_result27)
          {
            (*efp->pthis).v_list.first =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first;
            (*efp->pthis).v_list.last =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last;
            (*efp->pthis).v_list.length =
              (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length;
          }
        else
          {
            dealloc_v_list ((*((struct worker_data *) (*efp->_thr).wdptr)).
                            V_List.first);
            (*efp->pthis).v_list.first = 0;
            (*efp->pthis).v_list.last = 0;
            (*efp->pthis).v_list.length = 0;
          }
      }
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first = 0;
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.last = 0;
      (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length = 0;
    }
  else
    {
      (*efp->pthis).v_list.first = 0;
      (*efp->pthis).v_list.last = 0;
      (*efp->pthis).v_list.length = 0;
    }
}

struct do_many_bk4_in_search_p_c_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int j0_2;
  int j1_2;
  int ifexp_result31;
  int ifexp_result32;
  int ifexp_result29;
  int ifexp_result30;
  struct search_p_c_frame *xfp;
};

struct search_p_c_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  int spawned4;
  struct t_search_c *pthis;
  int J_end;
  int j2;
  int j;
  void *tmp4;
  int j1;
  int j0;
  struct v_list_body *my_v_list_cur;
  struct thread_data *_thr;
  closure_t *_bk;
  closure_t do_many_bk4;
};

char *do_many_bk4_in_search_p_c (char *esp, void *xfp0)
{
  struct do_many_bk4_in_search_p_c_frame *efp;
  struct search_p_c_frame *xfp = xfp0;
  char *parmp = (char *) esp;
  char *argp;

LGOTO:;
  efp = (struct do_many_bk4_in_search_p_c_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_many_bk4_in_search_p_c_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  if (!xfp->spawned4)
    {
      char *goto_fr;
      argp =
        (char *) ((Align_t *) esp +
                  (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
      *((closure_t **) argp) = xfp->_bk;
      goto_fr = lw_call (argp);
      if (goto_fr)
        if ((char *) goto_fr < (char *) efp)
          return goto_fr;
        else
          {
            efp->tmp_esp = 0;
            goto LGOTO;
          }
      else;
    }
  else;
  {
    if ((*xfp->_thr).treq_top)
      {
        if (xfp->j2 + 1 < xfp->J_end)
          efp->ifexp_result30 = 1;
        else
          efp->ifexp_result30 = 0;
        efp->ifexp_result29 = efp->ifexp_result30;
      }
    else
      efp->ifexp_result29 = 0;
    if (efp->ifexp_result29)
      {
        goto loop_start6;
        while (1)
          {
            if ((*xfp->_thr).treq_top)
              {
                if (xfp->j2 + 1 < xfp->J_end)
                  efp->ifexp_result32 = 1;
                else
                  efp->ifexp_result32 = 0;
                efp->ifexp_result31 = efp->ifexp_result32;
              }
            else
              efp->ifexp_result31 = 0;
            if (!efp->ifexp_result31)
              goto loop_end6;
            else;
          loop_start6:;
            efp->j1_2 = xfp->J_end;
            efp->j0_2 = (1 + xfp->j2 + xfp->J_end) / 2;
            xfp->J_end = efp->j0_2;
            xfp->pthis =
              (struct t_search_c *) malloc (sizeof (struct t_search_c));
            (*xfp->pthis).my_v_list_cur = xfp->my_v_list_cur;
            (*xfp->pthis).j0 = efp->j0_2;
            (*xfp->pthis).j1 = efp->j1_2;
            (xfp->spawned4)++;
            make_and_send_task (xfp->_thr, 3, xfp->pthis);
          }
      loop_end6:;
      }
    else;
  }
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void search_p_c (char *esp, closure_t * _bk, struct thread_data *_thr,
                 struct v_list_body *my_v_list_cur, int j0, int j1)
{
  struct search_p_c_frame *efp;

LGOTO:;
  efp = (struct search_p_c_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct search_p_c_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->_bk = _bk;
  efp->_thr = _thr;
  efp->my_v_list_cur = my_v_list_cur;
  efp->j0 = j0;
  efp->j1 = j1;
  efp->do_many_bk4.fun = do_many_bk4_in_search_p_c;
  efp->do_many_bk4.fr = (void *) efp;
  {
  }
  {
    efp->j2 = efp->j0;
    efp->J_end = efp->j1;
    efp->spawned4 = 0;
    if ((*efp->_thr).req)
      while (handle_req (esp, &efp->do_many_bk4, efp->_thr),
             __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
        {
          if ((char *) efp->tmp_esp < (char *) efp)
            return;
          else;
          efp->tmp_esp = 0;
          goto LGOTO;
        }
    else;
    for (; efp->j2 < efp->J_end; (efp->j2)++)
      {
        while (search1_p
               (esp, &efp->do_many_bk4, efp->_thr,
                ((*efp->my_v_list_cur).v)[efp->j2], 800, 0,
                (VV[((*efp->my_v_list_cur).v)[efp->j2]]).degree),
               __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
          {
            if ((char *) efp->tmp_esp < (char *) efp)
              return;
            else;
            efp->tmp_esp = 0;
            goto LGOTO;
          }
      }
    while ((efp->spawned4)-- > 0)
      {
        {
          while (__builtin_expect
                 ((efp->tmp4 = wait_rslt (esp, efp->_thr)) == (void *) 0 - 1, 0)
                 && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                      1))
            {
              if ((char *) efp->tmp_esp < (char *) efp)
                return;
              else;
              efp->tmp_esp = 0;
              goto LGOTO;
            }
          efp->pthis = (struct t_search_c *) efp->tmp4;
        }
        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.first =
          nconc_v_list ((*efp->pthis).v_list.first, (*efp->pthis).v_list.last,
                        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.
                        first);
        (*((struct worker_data *) (*efp->_thr).wdptr)).V_List.length +=
          (*efp->pthis).v_list.length;
        free (efp->pthis);
      }
  }
}

int make_edge1 (int v1, int v2, int e)
{
  (EE[e]).next_v = v2;
  (EE[e]).next_e = (VV[v1]).first_e;
  (VV[v1]).first_e = e++;
  ((VV[v1]).degree)++;
  return e;
}

int check_dup1 (int v1, int v2)
{
  int i;
  int d;
  int e;
  int nv;

  d = (VV[v1]).degree;
  e = (VV[v1]).first_e;
  {
    i = 0;
    for (; i < d; (i++, e = (EE[e]).next_e))
      {
        nv = (EE[e]).next_v;
        if (nv == v2)
          return 0;
        else;
      }
  }
  return 1;
}

int check_make_edge1 (int v1, int v2, int e)
{
  if (check_dup1 (v1, v2))
    {
      e = make_edge1 (v1, v2, e);
    }
  else;
  return e;
}

int make_edge (int v1, int v2, int e)
{
  e = make_edge1 (v1, v2, e);
  e = make_edge1 (v2, v1, e);
  return e;
}

int check_make_edge (int v1, int v2, int e)
{
  e = check_make_edge1 (v1, v2, e);
  e = check_make_edge1 (v2, v1, e);
  return e;
}

void make_bintree (int depth)
{
  int n;
  unsigned long next;
  int i;
  int e;
  int v[3];
  int p;
  int q;
  int n_e;

  n = (1 << depth) - 1;
  next = depth;
  fprintf (stderr, "Depth %d binary tree (%d vertices)\n", depth, n);
  {
    i = 0;
    for (; i < n; i++)
      {
        (VV[i]).degree = 0;
        (VV[i]).first_e = 0;
        (VV[i]).parent = 0;
      }
  }
  e = 0;
  {
    i = 0;
    for (; i < n; i++)
      {
        if (1)
          {
            if (i == 0)
              {
                n_e = 2;
                v[0] = i * 2 + 1;
                v[1] = i * 2 + 2;
              }
            else if (i < (n - 1) / 2)
              {
                n_e = 3;
                v[0] = (i - 1) / 2;
                v[1] = i * 2 + 1;
                v[2] = i * 2 + 2;
              }
            else
              {
                n_e = 1;
                v[0] = (i - 1) / 2;
              }
            next = next * 1103515245 + 12345;
            {
              q = n_e;
              for (; q > 0; q--)
                {
                  next = next * 1103515245 + 12345;
                  p = q * (next / 65536 % 32768) / 32768;
                  e = check_make_edge1 (i, v[p], e);
                  v[p] = v[q - 1];
                }
            }
          }
        else if (i > 0)
          e = make_edge (i, (i - 1) / 2, e);
        else;
      }
  }
  V_Length = n;
  E_Length = e;
}

void make_2dt (int n)
{
  int ix;
  int iy;
  int e;
  unsigned long next;
  int k;
  int k2;
  int px;
  int py;
  int sx;
  int sy;
  int v[4];
  int p;
  int q;

  next = n;
  fprintf (stderr, "%dx%d nodes 2D torus\n", n, n);
  {
    ix = 0;
    for (; ix < n; ix++)
      {
        iy = 0;
        for (; iy < n; iy++)
          {
            k = ix * n + iy;
            (VV[k]).degree = 0;
            (VV[k]).first_e = 0;
            (VV[k]).parent = 0;
          }
      }
  }
  e = 0;
  {
    ix = 0;
    for (; ix < n; ix++)
      {
        iy = 0;
        for (; iy < n; iy++)
          {
            k2 = ix * n + iy;
            px = ix - 1;
            py = iy - 1;
            sx = ix + 1;
            sy = iy + 1;
            if (px < 0)
              {
                px += n;
              }
            else;
            if (py < 0)
              {
                py += n;
              }
            else;
            if (sx >= n)
              {
                sx -= n;
              }
            else;
            if (sy >= n)
              {
                sy -= n;
              }
            else;
            {
              v[0] = ix * n + py;
              v[1] = px * n + iy;
              v[2] = ix * n + sy;
              v[3] = sx * n + iy;
              next = next * 1103515245 + 12345;
              {
                q = 4;
                for (; q > 0; --q)
                  {
                    next = next * 1103515245 + 12345;
                    p = q * (next / 65536 % 32768) / 32768;
                    e = check_make_edge1 (k2, v[p], e);
                    v[p] = v[q - 1];
                  }
              }
            }
          }
      }
  }
  V_Length = n * n;
  E_Length = e;
}

void make_ncube (int n)
{
  int i;
  int j;
  int s;
  int e;
  unsigned long next;
  int p;
  int q;
  int *v;

  s = 1 << n;
  e = 0;
  next = n;
  fprintf (stderr, "%d-D hypercube\n", n, n);
  {
    i = 0;
    for (; i < s; i++)
      {
        (VV[i]).degree = 0;
        (VV[i]).first_e = 0;
        (VV[i]).parent = 0;
      }
  }
  {
    i = 0;
    for (; i < s; i++)
      {
        v = (int *) malloc (n * sizeof (int));
        {
          j = 0;
          for (; j < n; j++)
            {
              v[j] = i ^ 1 << j;
            }
        }
        next = next * 1103515245 + 12345;
        {
          q = n;
          for (; q > 0; --q)
            {
              next = next * 1103515245 + 12345;
              p = q * (next / 65536 % 32768) / 32768;
              e = make_edge1 (i, v[p], e);
              v[p] = v[q - 1];
            }
        }
        free (v);
      }
  }
  V_Length = s;
  E_Length = e;
}

void make_random (int n, int m)
{
  int i;
  int j;
  int e;
  unsigned long next;
  int v;

  e = 0;
  next = n + m;
  fprintf (stderr, "%d nodes random (%d edges per node)\n", n, m);
  {
    i = 0;
    for (; i < n; i++)
      {
        (VV[i]).degree = 0;
        (VV[i]).first_e = 0;
        (VV[i]).parent = 0;
      }
  }
  {
    i = 0;
    for (; i < n; i++)
      {
        next = next * 1103515245 + 12345;
        {
          j = 0;
          for (; j < m; j++)
            {
              next = next * 1103515245 + 12345;
              v = n * (next / 65536 % 32768) / 32768;
              e = check_make_edge (i, v, e);
            }
        }
      }
  }
  V_Length = n;
  E_Length = e;
}

void show_graph ()
{
  int v;
  int e;
  int i;
  int d;
  int e2;

  {
    v = 0;
    for (; v < V_Length; v++)
      {
        fprintf (stderr, "%9d[%d] (", v, (VV[v]).parent - 1);
        {
          d = (VV[v]).degree;
          e2 = (VV[v]).first_e;
          if (d > 0)
            {
              fprintf (stderr, "%d", (EE[e2]).next_v);
              e2 = (EE[e2]).next_e;
              {
                i = 1;
                for (; i < d; (i++, e2 = (EE[e2]).next_e))
                  {
                    fprintf (stderr, ",%d", (EE[e2]).next_v);
                  }
              }
            }
          else;
        }
        fprintf (stderr, ")\n");
      }
  }
}

void show_graph2 ()
{
  int v;
  int e;
  int i;

  {
    v = 0;
    for (; v < V_Length; v++)
      {
        fprintf (stderr, "%9d[%d] e: %d\n", v, (VV[v]).degree, (VV[v]).first_e);
      }
  }
  {
    e = 0;
    for (; e < E_Length; e++)
      {
        fprintf (stderr, "%9d => %d -> e: %d\n", e, (EE[e]).next_v,
                 (EE[e]).next_e);
      }
  }
}

void dump_graph (char *name, char *file)
{
  int v;
  int e;
  FILE *fp;

  fp = fopen (file, "w");
  fprintf (fp, "%s\n", name);
  fprintf (fp, "vertex\n");
  fprintf (fp, "%d\n", V_Length);
  {
    v = 0;
    for (; v < V_Length; v++)
      {
        fprintf (fp, "%d %d\n", (VV[v]).degree, (VV[v]).first_e);
      }
  }
  fprintf (fp, "edge\n");
  fprintf (fp, "%d\n", E_Length);
  {
    e = 0;
    for (; e < E_Length; e++)
      {
        fprintf (fp, "%d %d\n", (EE[e]).next_v, (EE[e]).next_e);
      }
  }
  fclose (fp);
  fprintf (stderr, "Graph \"%s\" successfully dumped.\n", name);
  return;
}

void expect_int (char *str, int expct, int n)
{
  if (expct != n)
    {
      fprintf (stderr, "%s: expectes %d but %d", str, expct, n);
      exit (-1);
    }
  else;
  return;
}

void read_graph (char *file)
{
  int v;
  int e;
  char name[1024];
  char str[1024];
  FILE *fp;

  fp = fopen (file, "r");
  expect_int ("fscanf", 1, fscanf (fp, "%s", name));
  expect_int ("fscanf", 1, fscanf (fp, "%s", str));
  if (!(str[0] == 'v'))
    {
      fprintf (stderr, "File format error\n");
      exit (-1);
    }
  else;
  expect_int ("fscanf", 1, fscanf (fp, "%d", &V_Length));
  {
    v = 0;
    for (; v < V_Length; v++)
      {
        (VV[v]).parent = 0;
        expect_int ("fscanf", 2,
                    fscanf (fp, "%d %d", &(VV[v]).degree, &(VV[v]).first_e));
      }
  }
  expect_int ("fscanf", 1, fscanf (fp, "%s", str));
  if (!(str[0] == 'e'))
    {
      fprintf (stderr, "File format error\n");
      exit (-1);
    }
  else;
  expect_int ("fscanf", 1, fscanf (fp, "%d", &E_Length));
  {
    e = 0;
    for (; e < E_Length; e++)
      {
        expect_int ("fscanf", 2,
                    fscanf (fp, "%d %d", &(EE[e]).next_v, &(EE[e]).next_e));
      }
  }
  fclose (fp);
  fprintf (stderr, "Graph \"%s\" successfully loaded.\n", name);
  return;
}

void check_graph ()
{
  int v;

  {
    v = 0;
    for (; v < V_Length; v++)
      {
        if ((VV[v]).parent - 1 < 0 || (VV[v]).parent - 1 >= V_Length)
          {
            fprintf (stderr, "!! %9d[%d]\n", v, (VV[v]).parent - 1);
          }
        else;
      }
  }
}

struct spanning_start
{
  int graph;
  int algorithm;
  int show_graph_p;
  int show_tree_p;
  char _dummy_[1000];
};

struct _bk_in_do_spanning_start_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct do_spanning_start_task_frame *xfp;
};

struct do_spanning_start_task_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  char *gname;
  char *algstr;
  struct timeval tp[2];
  struct spanning_start *pthis;
  struct thread_data *_thr;
  closure_t _bk;
};

char *_bk_in_do_spanning_start_task (char *esp, void *xfp0)
{
  struct _bk_in_do_spanning_start_task_frame *efp;
  struct do_spanning_start_task_frame *xfp = xfp0;
  char *parmp = (char *) esp;

LGOTO:;
  efp = (struct _bk_in_do_spanning_start_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct _bk_in_do_spanning_start_task_frame) +
               sizeof (Align_t) + -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->xfp = xfp;
  {
    *((int *) efp) = 0;
    return 0;
  }
  return 0;
}

void do_spanning_start_task (char *esp, struct thread_data *_thr,
                             struct spanning_start *pthis)
{
  struct do_spanning_start_task_frame *efp;

LGOTO:;
  efp = (struct do_spanning_start_task_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct do_spanning_start_task_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  efp->_thr = _thr;
  efp->pthis = pthis;
  efp->_bk.fun = _bk_in_do_spanning_start_task;
  efp->_bk.fr = (void *) efp;
  fprintf (stderr, "CALL-BOUND: %d\n", 800);
  fprintf (stderr, "BARRIER: %s\n", "type 1 (cas)");
  gettimeofday (efp->tp, 0);
  switch ((*efp->pthis).graph)
    {
    case 1:
      make_random (2000 * 2000, 2);
      efp->gname = "Random4000000.graph";
      break;
    case 2:
      make_2dt (2000);
      efp->gname = "2DTorus.graph";
      break;
    case 3:
      make_ncube (20);
      efp->gname = "HCube20.graph";
      break;
    case 4:
      make_bintree (24);
      efp->gname = "Bintree24.graph";
      break;
    case 5:
      make_ncube (21);
      efp->gname = "HCube21.graph";
      break;
    case 11:
      efp->gname = "Random4000000.graph";
      read_graph (efp->gname);
      break;
    case 12:
      efp->gname = "2DTorus.graph";
      read_graph (efp->gname);
      break;
    case 13:
      efp->gname = "HCube20.graph";
      read_graph (efp->gname);
      break;
    case 14:
      efp->gname = "Bintree24.graph";
      read_graph (efp->gname);
      break;
    case 15:
      efp->gname = "HCube21.graph";
      read_graph (efp->gname);
      break;
    default:
      make_random (2000 * 2000, 2);
      efp->gname = "Random4000000.graph";
      break;
    }
  gettimeofday (efp->tp + 1, 0);
  fprintf (stderr, "Graph constuction time: %lf\n", elapsed_time (efp->tp));
  fprintf (stderr, "v: %d e: %d\n", V_Length, E_Length);
  switch ((*efp->pthis).show_graph_p)
    {
    case 1:
      show_graph ();
      break;
    case 2:
      show_graph2 ();
      break;
    }
  gettimeofday (efp->tp, 0);
  switch ((*efp->pthis).algorithm)
    {
    case 1:
      search_s2 (0);
      efp->algstr = "search-s2";
      break;
    case 2:
      search (0);
      efp->algstr = "search";
      break;
    case 3:
      while (search_p (esp, &efp->_bk, efp->_thr, 0),
             __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
        {
          if ((char *) efp->tmp_esp < (char *) efp)
            return;
          else;
          efp->tmp_esp = 0;
          goto LGOTO;
        }
      efp->algstr = "search-p";
      break;
    case 4:
      while (search_p_array (esp, &efp->_bk, efp->_thr, 0),
             __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
        {
          if ((char *) efp->tmp_esp < (char *) efp)
            return;
          else;
          efp->tmp_esp = 0;
          goto LGOTO;
        }
      efp->algstr = "search-p-array";
      break;
    case 99:
      dump_graph (efp->gname, efp->gname);
      exit (0);
    default:
      search_s2 (0);
      efp->algstr = "search-s2";
      break;
    }
  gettimeofday (efp->tp + 1, 0);
  fprintf (stderr, "%s finished.\ntime: %lf\n", efp->algstr,
           elapsed_time (efp->tp));
  switch ((*efp->pthis).show_tree_p)
    {
    case 1:
      show_graph ();
      break;
    case 2:
      show_graph2 ();
      break;
    }
  check_graph ();
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

void send_spanning_start_task (struct spanning_start *pthis)
{
  send_int ((*pthis).graph);
  send_int ((*pthis).algorithm);
  send_int ((*pthis).show_graph_p);
  send_int ((*pthis).show_tree_p);
}

struct spanning_start *recv_spanning_start_task ()
{
  struct spanning_start *pthis;
  pthis = malloc (sizeof (struct spanning_start));
  (*pthis).graph = recv_int ();
  (*pthis).algorithm = recv_int ();
  (*pthis).show_graph_p = recv_int ();
  (*pthis).show_tree_p = recv_int ();
  return pthis;
}

void send_spanning_start_rslt (struct spanning_start *pthis)
{
  free (pthis);
}

void recv_spanning_start_rslt (struct spanning_start *pthis)
{
}

void send_t_search_c_task (struct t_search_c *pthis)
{
  ;
  send_int ((*pthis).j0);
  send_int ((*pthis).j1);
}

struct t_search_c *recv_t_search_c_task ()
{
  struct t_search_c *pthis;
  pthis = malloc (sizeof (struct t_search_c));
  ;
  (*pthis).j0 = recv_int ();
  (*pthis).j1 = recv_int ();
  return pthis;
}

void send_t_search_c_rslt (struct t_search_c *pthis)
{
  ;
  free (pthis);
}

void recv_t_search_c_rslt (struct t_search_c *pthis)
{
  ;
}

void send_t_search_b_array_task (struct t_search_b_array *pthis)
{
  ;
  send_int ((*pthis).i0);
  send_int ((*pthis).i1);
}

struct t_search_b_array *recv_t_search_b_array_task ()
{
  struct t_search_b_array *pthis;
  pthis = malloc (sizeof (struct t_search_b_array));
  ;
  (*pthis).i0 = recv_int ();
  (*pthis).i1 = recv_int ();
  return pthis;
}

void send_t_search_b_array_rslt (struct t_search_b_array *pthis)
{
  ;
  free (pthis);
}

void recv_t_search_b_array_rslt (struct t_search_b_array *pthis)
{
  ;
}

void send_t_search_b_task (struct t_search_b *pthis)
{
  ;
  send_int ((*pthis).i0);
  send_int ((*pthis).i1);
}

struct t_search_b *recv_t_search_b_task ()
{
  struct t_search_b *pthis;
  pthis = malloc (sizeof (struct t_search_b));
  ;
  (*pthis).i0 = recv_int ();
  (*pthis).i1 = recv_int ();
  return pthis;
}

void send_t_search_b_rslt (struct t_search_b *pthis)
{
  ;
  free (pthis);
}

void recv_t_search_b_rslt (struct t_search_b *pthis)
{
  ;
}

void send_t_search1_task (struct t_search1 *pthis)
{
  send_int ((*pthis).v);
  send_int ((*pthis).b);
  send_int ((*pthis).k0);
  send_int ((*pthis).k1);
}

struct t_search1 *recv_t_search1_task ()
{
  struct t_search1 *pthis;
  pthis = malloc (sizeof (struct t_search1));
  (*pthis).v = recv_int ();
  (*pthis).b = recv_int ();
  (*pthis).k0 = recv_int ();
  (*pthis).k1 = recv_int ();
  return pthis;
}

void send_t_search1_rslt (struct t_search1 *pthis)
{
  ;
  free (pthis);
}

void recv_t_search1_rslt (struct t_search1 *pthis)
{
  ;
}

void (*task_doers[256]) (char *, struct thread_data *, void *) =
{
(void (*)(char *, struct thread_data *, void *)) do_t_search1_task,
    (void (*)(char *, struct thread_data *, void *)) do_t_search_b_task,
    (void (*)(char *, struct thread_data *, void *)) do_t_search_b_array_task,
    (void (*)(char *, struct thread_data *, void *)) do_t_search_c_task,
    (void (*)(char *, struct thread_data *, void *)) do_spanning_start_task};
void (*task_senders[256]) (void *) =
{
(void (*)(void *)) send_t_search1_task,
    (void (*)(void *)) send_t_search_b_task,
    (void (*)(void *)) send_t_search_b_array_task,
    (void (*)(void *)) send_t_search_c_task,
    (void (*)(void *)) send_spanning_start_task};
void *(*task_receivers[256]) () =
{
(void *(*)()) recv_t_search1_task, (void *(*)()) recv_t_search_b_task,
    (void *(*)()) recv_t_search_b_array_task,
    (void *(*)()) recv_t_search_c_task, (void *(*)()) recv_spanning_start_task};
void (*rslt_senders[256]) (void *) =
{
(void (*)(void *)) send_t_search1_rslt,
    (void (*)(void *)) send_t_search_b_rslt,
    (void (*)(void *)) send_t_search_b_array_rslt,
    (void (*)(void *)) send_t_search_c_rslt,
    (void (*)(void *)) send_spanning_start_rslt};
void (*rslt_receivers[256]) (void *) =
{
(void (*)(void *)) recv_t_search1_rslt,
    (void (*)(void *)) recv_t_search_b_rslt,
    (void (*)(void *)) recv_t_search_b_array_rslt,
    (void (*)(void *)) recv_t_search_c_rslt,
    (void (*)(void *)) recv_spanning_start_rslt};