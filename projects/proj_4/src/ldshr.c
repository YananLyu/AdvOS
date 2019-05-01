#include "ldshr.h"
#include <ctype.h>
#include <math.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

CLIENT *clients[10];
FILE   *fp = NULL;
char *clnt_name[10];
int minLoadCLientIndex =0;
int CLIENT_COUNT=0;

struct load_dict{
    CLIENT *clients;
    char *clnt_name;
    double loadavg;
};

void analyse_clnt_load(){

    
    char clientName[10];

      if (!(fp = fopen(MACHINEFILE, "r")))
      {
          printf("Error opening machinefile\n");
          exit(1);
      }
      while ( fgets ( clientName, sizeof clientName, fp ) != NULL ) /* read a line */
      {
          clientName[strcspn(clientName, "\n")] = '\0';
          clnt_name[CLIENT_COUNT] = malloc(strlen(clientName));
          strcpy(clnt_name[CLIENT_COUNT] , clientName);
          CLIENT_COUNT=CLIENT_COUNT+1;
      }
      fclose (fp);
      
    int index;
    struct load_dict load_clnt[CLIENT_COUNT];           
    

    for(index=0;index<CLIENT_COUNT;index++)
    {
        if(!(clients[index] = clnt_create(clnt_name[index],RDBPROG,RDBVERS,"tcp"))){
            clnt_pcreateerror(clnt_name[index]);
            exit(1);
        }
    }

    for(index=0;index<CLIENT_COUNT;index++)
    {
        double val = -1;
        load_clnt[index].clients = clients[index];
        load_clnt[index].clnt_name = clnt_name[index];
        load_clnt[index].loadavg = *(get_load_1(NULL, load_clnt[index].clients));

    }
 
    for(index=0;index<CLIENT_COUNT;index++){
        if(load_clnt[index].loadavg < load_clnt[minLoadCLientIndex].loadavg)
            {
                minLoadCLientIndex = index;
            }
            printf("%s: %.3f\t", clnt_name[index], load_clnt[index].loadavg);
        }
        printf("\n");
        printf("(executed on %s)\n",clnt_name[minLoadCLientIndex]);
        
}

void addNode(struct Node** head, double value)
{
    struct Node * newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->value = value;
    newNode->next = (*head);
    (*head) = newNode;
}

void displayLinkedList(struct Node *node) 
{ 
    while (node != NULL) 
    { 
        printf(" %.1f\t", node->value);
        node = node->next; 
    } 
} 



int main(int argc, char *argv[])
{
    int maxFlag = 0;
    int updFlag = 0;

    char command[4] = "";
    if(argc<2){
	  	 printf("Too few arguments..\n");
	}
    else
    {
        strcpy(command,argv[1]);

        if (strcmp("-max", command) == 0)
        {
            maxFlag = 1;
        }        
        else if (argc > 2 && strcmp("-upd", command) == 0)
        {
            updFlag = 1;
        }
        else
        {
            printf("Invalid option specified\n");
            exit(1);
        }
        
        
        analyse_clnt_load();

        if(maxFlag == 1)
        {
            struct max_para para;
            para.n = atoi(argv[2]);
            para.mean = atoi(argv[3]);
            para.seed = atoi(argv[4]);
            double *max = findmax_gpu_1(&para, clients[minLoadCLientIndex]);
            printf("%f\n", *max);
        }    
        else if(updFlag == 1)
        {
            struct Node *head = NULL;
            int index=argc-1;
            while( index > 1)
            {
                addNode(&head,atof(argv[index]));
                index = index-1;
            }
           
           struct Node *updatedlist =  update_lst_1(head,clients[minLoadCLientIndex]);
           displayLinkedList(updatedlist);
           printf("\n");
        }
    }
    
    
} 
