#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <pthread.h>

#define NUM_THREADS 4

long long total = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* mont(void* data) { 
    int num = *((int*) data);
    
    srand(time(NULL));

    long long sum = 0;
    for(int i = 1; i < num; i++) { 
        double x = (double) rand() / RAND_MAX; 
        double y = (double) rand() / RAND_MAX;
        if((x * x + y * y) < 1) {
            sum++; 
        }
    } 
    
    pthread_mutex_lock(&mutex);
    total += sum;
    pthread_mutex_unlock(&mutex);
}  

int main(){
    int num_points;
    double pi_estimate;
    pthread_t threads[NUM_THREADS];
    
    printf("Point number = ");
    scanf("%d", &num_points);
    
    int points_per_thread[NUM_THREADS];
    int remainder = num_points % NUM_THREADS;
    int quotient = num_points / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++){
        points_per_thread[i] = (i < remainder) ? quotient + 1 : quotient;
    }

    for (int i = 0; i < NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, mont, (void*) &points_per_thread[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    pi_estimate = (double) total * 4 / num_points;
    printf("%f\n", pi_estimate);
    
    return 0;
}