#include <stdio.h>
#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/slab.h>
#include <linux/init.h> 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NTR");
MODULE_DESCRIPTION("Project_1");

void push(struct_birthday**,int,int,int);
struct_birthday* delete (struct_birthday** ,struct_birthday* );
struct_birthday * create_new_node(int,int,int );
void Traversal(const struct_birthday** );

typedef  struct struct_birthday {
    int month;
    int year;
    int gender;
    struct_birthday * next_node ;
}struct_birthday;

struct_birthday * create_new_node(int month,int year,int gender){
    struct_birthday * node = (struct_birthday *)kmalloc(sizeof(struct_birthday),GFP_KERNEL);
    node->month=month;
    node->year=year;
    node->gender = gender;
    node->next_node = NULL;
    return node;
}
void push(struct_birthday* list,int month,int year,int gender){
    struct_birthday * node = create_new_node(month,year); 
    
    if (!list){
        list = node;
        return;
    }

    struct_birthday* itr = list;
    while(itr->next_node){
        itr = itr->next_node;
    }
    itr->next_node = node ; 
}
struct_birthday* delete (struct_birthday* list ,struct_birthday* node){
    struct_birthday* prev = NULL;
    struct_birthday* cur = list;

    if (!cur){
        printf("List is empty !\n");
        return NULL;
    }
    if (cur == node){
        list = list->next_node;
        kfree(cur);
        return  list;
    }
    else {
        while(cur->next_node != node){
            prev = cur;
            cur = cur->next_node;
        }
        prev->next_node = cur->next_node;
        kfree(cur);
        return list;
    }
}
void Traversal(const struct_birthday* list){
    struct_birthday* itr = list;
    if (!(itr->next_node)){
        printf("YY/DD : %d/%d \n",itr->year,itr->month);
        return;
    }
    while(itr->next_node){
        printf("YY/DD : %d/%d \n",itr->year,itr->month);
        itr = itr->next_node;
    }
}
static int __init birthday_init(void)
{
    struct_birthday *result = NULL;

    push(&result ,2002, 5, 1);
    push(&result ,2020, 7, 21);
    push(&result ,2001, 3, 4);
    push(&result ,2010, 6, 1);
    push(&result ,2100, 1, 1);
    push(&result ,2102, 12, 1);

    printk(KERN_INFO "Birthday INIT.\n");

    Traversal(&result);

    return 0;    // Non-zero return means that the module couldn't be loaded.
}
static void __exit birthday_cleanup(void)
{
    clean(head);
    printk(KERN_INFO "Cleaning up module.\n");
}
module_init(birthday_init);
module_exit(birthday_cleanup);