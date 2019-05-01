#include "ldshr.h"
#include <math.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>

double reduceMax(int nValue, int meanValue, int seedValue);

double *get_load_1_svc(void *dummyArg, struct svc_req *req) {
  double *loadAverage = malloc(sizeof(double *));
  getloadavg(loadAverage, 1);
  return loadAverage;
}

double * findmax_gpu_1_svc(struct max_para * para, struct svc_req * req){

  double *max = malloc(sizeof(double *));
	*max = reduceMax(para->n,para->mean,para->seed);
	return max;
}
void scorecard(double * result)
{
    *result = sqrt(*result) *10;
}

struct Node * map(void (*f)(double *), struct Node *node)
{
    double result;
    struct Node *head = node;
    while (node != NULL) 
    { 
        (*f)(&node->value);
        node = node->next; 
    } 
    return head;
}

struct Node * update_lst_1_svc(struct Node * llist, struct svc_req * req){
      struct Node *updatedList = malloc(sizeof(Node *));
      updatedList = map(scorecard,llist);
      return updatedList;
}

