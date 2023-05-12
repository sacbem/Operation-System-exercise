#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   

#define PAGE_COUNT 10
#define PAGE_RANGE 10
#define STAGING_QUEUE_SIZE 5
#define DEFAULT -1
typedef struct stack frame_stack_t;
typedef struct linkedlist node_t;

struct node_t {
    int idx;
    int lhu_prior;
    node_t * next;
    void (*create_node)(node_t**,int);
    void (*print) (node_t*);
};
struct frame_stack_t{
    node_t* list;

    void (*init) (frame_stack_t**);
    int  (*push_back) (frame_stack_t**);
    int  (*pop) (frame_stack_t**);
    void (*create_node)(frame_stack_t**);
    void (*print) (frame_stack_t*);
};
void init(frame_stack_t**);
void create_node(node_t** ,int );
void create_node(frame_stack_t** );
void print(node_t*);
void print(frame_stack_t*);
int *create_page_ref_list();
int scheduling (node_t *);
int lhu_algo();

int* ref_list;
void create_node(node_t** top,int idx){

    node_t* node = malloc(sizeof(node));
    node->next = *top;
    node->idx = idx;
    node->lhu_prior = idx == 0 ? 1 : 0 ;
    *top = node;

}
void create_node(frame_stack_t** q){
    for (int i = STAGING_QUEUE_SIZE ; i > 0 ; i--){
        q->list->create_node(q->&list,i);
    }
}
int scheduling (node_t** n){
    node * tmp  = *n;
    while (*n->next){
        if()
    }
}
void init (node_t * n){
    n->create_node = create_node;
    n->print = print;
}

void init (frame_stack_t * q){
    q->list = NULL;
    q->create_node = create_node;
    q->push_back = push_back;
    q->replace = replace ;
    q->print = print;
}
int *create_page_ref_list(){
    int * list = malloc(sizeof(int) * PAGE_COUNT);
    srand(time(NULL));

    for (int i = 0 ; i < PAGE_COUNT ; i++){
        list[i] = rand() % PAGE_RANGE ;
    }
    return list;
}
int main (){

}