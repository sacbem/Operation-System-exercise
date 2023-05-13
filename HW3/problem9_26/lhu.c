#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PAGE_COUNT 10
#define PAGE_RANGE 10
#define STAGING_QUEUE_SIZE 5
int* ref_list;
typedef struct node_t node_t;
typedef struct frame_stack_t frame_stack_t;

struct node_t {
    int idx;
    int lhu_prior;
    node_t* next;
};

struct frame_stack_t {
    node_t* list;

    void (*init)(frame_stack_t*);
    void (*create_node)(node_t**, int);
    void (*create_node_s)(frame_stack_t*);
    void (*print_s)(frame_stack_t*);
    void (*free_node)(frame_stack_t*);
    int (*replace)(frame_stack_t*, int);
};

void init(frame_stack_t*);
void create_node(node_t**, int);
void create_node_s(frame_stack_t*);
void print_n(node_t*);
void print_s(frame_stack_t*);
void free_node(frame_stack_t*);
int  replace(frame_stack_t*, int);
int  notfound(frame_stack_t*, int);
int* create_page_ref_list();
int  lhu_algo(frame_stack_t*);

void create_node(node_t** top, int idx) {
    node_t* node = malloc(sizeof(node_t));
    node->next = *top;
    node->idx = idx;
    node->lhu_prior = 0;
    *top = node;
}

void create_node_s(frame_stack_t* self) {
    for (int i = STAGING_QUEUE_SIZE -1; i >= 0; i--) {
        create_node(&(self->list), ref_list[i]);
        if (i == 0) {
            self->list->lhu_prior = 1;
        }
    }
}

void print_n(node_t* self) {
    node_t* tmp = self;
    while (tmp) {
        printf("idx: %d, prior: %d\n", tmp->idx, tmp->lhu_prior);
        tmp = tmp->next;
    }
}

void print_s(frame_stack_t* self) {
    print_n(self->list);
}

void free_node(frame_stack_t* self) {
    node_t* tmp = self->list;
    while (tmp != NULL) {
        node_t* next = tmp->next;
        free(tmp);
        tmp = next;
    }
}

int replace(frame_stack_t* self, int tar) {
    node_t* tmp = self->list;
    for (int i = 0; i < STAGING_QUEUE_SIZE; i++) {
        if (tmp->lhu_prior) {
            //printf("idx: %d, tar: %d\n", tmp->idx, tar);
            tmp->idx = tar;
            tmp->lhu_prior = 0;

            tmp = tmp->next;
            tmp->lhu_prior = 1;
            return 1;
        }
        tmp = tmp->next;
    }
    return 0;
}

void init(frame_stack_t* self) {
    self->list = NULL;
    self->create_node = create_node;
    self->create_node_s = create_node_s;
    self->print_s = print_s;
    self->free_node = free_node;
    self->replace = replace;

    self->create_node_s(self);
}

int notfound(frame_stack_t* stack, int tar) {
    node_t* tmp = stack->list;
    for (int i = 0; i < STAGING_QUEUE_SIZE; i++) {
        if (tmp->idx == tar) {
            return 0;
        }
        tmp = tmp->next;
    }
    return 1;
}
int lhu_algo(frame_stack_t* stack) {
    int pg_fault = 0;
    for (int i = STAGING_QUEUE_SIZE; i < PAGE_COUNT; i++) {
        if (notfound(stack, ref_list[i])) {
            if (!stack->replace(stack, ref_list[i])) {
                printf("Page replacement failed for page %d\n", ref_list[i]);
                continue;
            }
            pg_fault++;
        }
    }
    printf("Page faults: %d\n", pg_fault);
    return pg_fault;
}

int* create_page_ref_list() {
    int* list = malloc(sizeof(int) * PAGE_COUNT);
    srand(time(NULL));

    for (int i = 0; i < PAGE_COUNT; i++) {
        list[i] = rand() % PAGE_RANGE;
    }
    return list;
}

int main() {
    frame_stack_t stack;
    ref_list = create_page_ref_list();

    printf("Reference list: ");
    for (int i = 0; i < PAGE_COUNT; i++) {
        printf("%d ", ref_list[i]);
    }
    printf("\n");

    init(&stack);
    stack.print_s(&stack);
    lhu_algo(&stack);
    stack.print_s(&stack);

    free(ref_list);
    stack.free_node(&stack);

    return 0;
}