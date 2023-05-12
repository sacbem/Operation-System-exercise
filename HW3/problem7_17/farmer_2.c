#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

pthread_mutex_t bridge_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t southbound_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t northbound_cond = PTHREAD_COND_INITIALIZER;

int southbound_count = 0;
int northbound_count = 0;

bool is_bridge_empty = true;
bool is_southbound_turn = true; // 輪流用 token

void enter_bridge(bool is_southbound) {

    if (is_southbound) {
        while (!is_bridge_empty || northbound_count > 0 || !is_southbound_turn) {  // 防止不長眼(有對向農民、橋有人)的農民上橋
            pthread_cond_wait(&southbound_cond, &bridge_mutex);
        }
        is_southbound_turn = false;
        southbound_count++;
    } else {
        while (!is_bridge_empty || southbound_count > 0 || is_southbound_turn) {
            pthread_cond_wait(&northbound_cond, &bridge_mutex);
        }
        is_southbound_turn = true;
        northbound_count++;
    }

    is_bridge_empty = false;
}
void exit_bridge(bool is_southbound) {
    if (is_southbound) {
        southbound_count--;
        is_bridge_empty = (southbound_count == 0 && northbound_count == 0);

        pthread_cond_broadcast(&northbound_cond);
        pthread_cond_signal(&southbound_cond);
    } else {
        northbound_count--;
        is_bridge_empty = (southbound_count == 0 && northbound_count == 0);

        pthread_cond_broadcast(&southbound_cond);
        pthread_cond_signal(&northbound_cond);
    }

    pthread_mutex_unlock(&bridge_mutex);
}

void cross_bridge_southbound() {
    pthread_mutex_lock(&bridge_mutex);

    enter_bridge(true);

    pthread_mutex_unlock(&bridge_mutex);

    printf("Southbound farmer is crossing the bridge.\n");
    sleep(1); 

    pthread_mutex_lock(&bridge_mutex);

    exit_bridge(true);
}

void cross_bridge_northbound() {

    pthread_mutex_lock(&bridge_mutex);
    
    enter_bridge(false);

    pthread_mutex_unlock(&bridge_mutex);

    printf("Northbound farmer is crossing the bridge.\n");
    sleep(1); 

    pthread_mutex_lock(&bridge_mutex);

    exit_bridge(false);
}

void crossing_farmer(void *arg){
    bool is_south = (bool)arg;

    while (1){
        if (is_south){
            cross_bridge_southbound();
        }
        else{
            cross_bridge_northbound();
        }
    }
}
int main() {
    pthread_t northbound_thread, southbound_thread;
    bool is_south = true;

    pthread_create(&northbound_thread, NULL, crossing_farmer, ((void *)!is_south));
    pthread_create(&southbound_thread, NULL, crossing_farmer, (void *)is_south);

    pthread_join(northbound_thread, NULL);
    pthread_join(southbound_thread, NULL);

    return 0;
}
