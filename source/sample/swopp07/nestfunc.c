#include<stdio.h>
typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double Align_t;

char* lw_call (char *esp); 

char *
lw_call (char *esp)
{
  closure_t *clos = *((closure_t **) esp);

  return (clos->fun) (esp, clos->fr);
}
