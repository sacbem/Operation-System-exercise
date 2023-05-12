#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10

int data[SIZE] = {9, 4, 6, 7, 2, 1, 5, 8, 3, 0};
int sorted_data[SIZE];

typedef struct {
    int start_index;
    int end_index;
} thread_args;

void *sort_thread(void *arg) {
    thread_args *args = (thread_args*) arg;
    int i, j;
    for (i = args->start_index; i < args->end_index; i++) {
        for (j = i + 1; j < args->end_index; j++) {
            if (data[i] > data[j]) {
                int temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
    pthread_exit(NULL);
}

void *merge_thread(void *arg) {
    int i = 0, j = SIZE / 2, k = 0;
    while (i < SIZE / 2 && j < SIZE) {
        if (data[i] < data[j]) {
            sorted_data[k++] = data[i++];
        } else {
            sorted_data[k++] = data[j++];
        }
    }
    while (i < SIZE / 2) {
        sorted_data[k++] = data[i++];
    }
    while (j < SIZE) {
        sorted_data[k++] = data[j++];
    }
    pthread_exit(NULL);
}

int main() {
    int i;
    pthread_t threads[3];
    thread_args args[2];

    // Create threads to sort the two halves of the array
    args[0].start_index = 0;
    args[0].end_index = SIZE / 2;
    pthread_create(&threads[0], NULL, sort_thread, &args[0]);

    args[1].start_index = SIZE / 2;
    args[1].end_index = SIZE;
    pthread_create(&threads[1], NULL, sort_thread, &args[1]);

    // Wait for the two sort threads to finish
    for (i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    // Create a thread to merge the two sorted halves of the array
    pthread_create(&threads[2], NULL, merge_thread, NULL);
    pthread_join(threads[2], NULL);

    // Output the sorted array
    printf("Sorted array: ");
    for (i = 0; i < SIZE; i++) {
        printf("%d ", sorted_data[i]);
    }
    printf("\n");

    return 0;
}

