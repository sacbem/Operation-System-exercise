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
        while (!is_bridge_empty || northbound_count > 0 || !is_southbound_turn) {
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

    // Simulating crossing the bridge
    printf("Southbound farmer is crossing the bridge.\n");
    sleep(1); 

    pthread_mutex_lock(&bridge_mutex);

    exit_bridge(true);
}

void cross_bridge_northbound() {
    pthread_mutex_lock(&bridge_mutex);

    enter_bridge(false);

    pthread_mutex_unlock(&bridge_mutex);

    // Simulating crossing the bridge
    printf("Northbound farmer is crossing the bridge.\n");
    sleep(1); 

    pthread_mutex_lock(&bridge_mutex);

    exit_bridge(false);
}

void *southbound_farmer(void *arg) {
    while (true) {
        cross_bridge_southbound();
    }
}

void *northbound_farmer(void *arg) {
    while (true) {
        cross_bridge_northbound();
    }
}

int main() {
    pthread_t northbound_thread, southbound_thread;

    // Create the threads for northbound and southbound farmers
    pthread_create(&northbound_thread, NULL, northbound_farmer, NULL);
    pthread_create(&southbound_thread, NULL, southbound_farmer, NULL);

    // Wait for the threads to finish (This part may not be reached in this program)
    pthread_join(northbound_thread, NULL);
    pthread_join(southbound_thread, NULL);

    return 0;
}
