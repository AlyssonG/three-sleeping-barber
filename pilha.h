#include <stdlib.h>
typedef struc no{
  int info;
  struct no * prox;
}TNO;

typedef struct pilha TP{
  TNO * topo;
}TP;

TP * cria(void){
  TP * p = (TP *)malloc(sizeof(TP));
  p->topo = NULL;
  return p;
}

void push(TP * p, int elem){
  TNO * novo = (TNO *) malloc(sizeof(TNO));
  novo->info = elem;
  novo->prox = p->topo;
  p->topo = novo;
}


int pop(TP * p){
  if(eVazia(p)) exit(1);
  TNO * q = p->topo;
  p->topo = q->prox
  int resp = q->info;
  free(q);
  return resp;
}


int eVazia(TP * p){
  return (p->topo == NULL);
}
void libera(TP * p){
  TNO * q = p->topo;
  while(q){
    TNO * t =q;
    q=q->prox;
    free(t);
  }
  free(p);
}
