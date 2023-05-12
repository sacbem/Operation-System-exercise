#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <pthread.h>

#define POINT_NUM 10000001
double result;

void* monte(void* data) {     
    srand(time(NULL));     
    int inCircle = 0;
    
    for(int i = 0; i < POINT_NUM; i++) { 
        double x = (double) rand() / RAND_MAX; 
        double y = (double) rand() / RAND_MAX;
        
        if((x * x + y * y) < 1) {
            inCircle++; 
        }
    } 

    result = (double) 4 * inCircle / POINT_NUM;
    printf("pi = %f\n", result);
    return NULL;
}  

int main(){
    pthread_t t;
    pthread_create(&t, NULL, monte, NULL);
    pthread_join(t, NULL);
    return 0;
}