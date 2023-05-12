#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   

#define PAGE_COUNT 10
#define PAGE_RANGE 10
#define STAGING_QUEUE_SIZE 5
#define DEFAULT -1
typedef struct staging_queue_t staging_queue_t ;
struct staging_queue_t{
    int queue[STAGING_QUEUE_SIZE];
    int last_enter_idx;
    int tail;

    void (* init) (staging_queue_t*);
    void (* push_back) (staging_queue_t*,int);
    void (* replace) (staging_queue_t*,int);
    void (* print) (staging_queue_t*);
};

int* create_page_ref_list();
int  not_found(int *,int);
void init(staging_queue_t*);
void push_back(staging_queue_t*,int);
void replace(staging_queue_t*,int);
void print(staging_queue_t* );
int  fifo_algo(staging_queue_t* );
int* ref_list;

int *create_page_ref_list(){
    int * list = malloc(sizeof(int) * PAGE_COUNT);
    srand(time(NULL));

    for (int i = 0 ; i < PAGE_COUNT ; i++){
        list[i] = rand() % PAGE_RANGE ;
    }
    return list;
}

void push_back(staging_queue_t* self, int item){
    self->queue[self->tail] = item;
    if(self->tail < STAGING_QUEUE_SIZE ){
        self->tail++;
    }
}

void replace(staging_queue_t* self, int item){
    self->queue[self->last_enter_idx] = item;
    self->last_enter_idx++;
    self->last_enter_idx %= STAGING_QUEUE_SIZE;
}
void print(staging_queue_t* self){
    printf("Queue Content : ");
    for(int i = 0 ; i < self->tail; i++){
        printf("%d ",self->queue[i]);
    }
    printf("\n");
}
int not_found(int *q,int target){
    for (int i = 0 ; i < STAGING_QUEUE_SIZE ; i++){
        if(q[i] == target){
            return 0;
        }
    }
    return 1;
}

int fifo_algo(staging_queue_t* q){
    int pg_fault = 0 ;

    for (int i = 0 ; i < PAGE_COUNT ; i++ ){
        if (not_found(q->queue,ref_list[i])){
            if(q->tail == STAGING_QUEUE_SIZE){ // full
                q->replace(q,ref_list[i]);
            }
            else{ // not full
                q->push_back(q,ref_list[i]);
            }
            pg_fault++;
        }
        q->print(q);
    }
    return pg_fault;
}

void init(staging_queue_t* self){

   for(int i = 0 ; i < STAGING_QUEUE_SIZE ; i++){
        self->queue[i] = DEFAULT;
   }
   
   self->last_enter_idx = self->tail = 0;
   self->push_back = push_back;
   self->replace = replace;
   self->print = print;
}

int main(){
    ref_list = create_page_ref_list();

    printf("refence list : ");
    for(int i = 0 ;i < PAGE_COUNT ; i++){
        printf("%d ", ref_list[i]);
    }
    printf("\n");

    staging_queue_t fifo_queue;
    fifo_queue.init = init;

    fifo_queue.init(&fifo_queue);
    fifo_algo(&fifo_queue);
    free(ref_list);   
    return 0;
}

/** test seg.
    for(int i = 0 ;i < STAGING_QUEUE_SIZE ; i++){
        printf("%d ", fifo_queue.queue[i]);
    }
    printf("\n");
*/