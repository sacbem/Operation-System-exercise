#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 27 // 9行 + 9列 + 9區域

int sudoku[9][9] = { {6, 2, 4, 5, 3, 9, 1, 8, 7},
                     {5, 1, 9, 7, 2, 8, 6, 3, 4},
                     {8, 3, 7, 6, 1, 4, 2, 9, 5},
                     {1, 4, 3, 8, 6, 5, 7, 2, 9},
                     {9, 5, 8, 2, 4, 7, 3, 6, 1},
                     {7, 6, 2, 3, 9, 1, 4, 5, 8},
                     {3, 7, 1, 9, 5, 6, 8, 4, 2},
                     {4, 9, 6, 1, 8, 2, 5, 7, 3},
                     {2, 8, 5, 4, 7, 3, 9, 1, 6} };

int valid[27] = {0}; // 線程返回值，用於檢查數獨解是否有效

void *check_row(void *arg) {
    int row = *((int *) arg);
    int count[10] = {0}; // 計數器
    for (int i = 0; i < 9; i++) {
        if (sudoku[row][i] == 0) {
            continue; // 忽略空單元格
        }
        count[sudoku[row][i]]++;
        if (count[sudoku[row][i]] > 1) {
            valid[row] = 0;
            return NULL; // 重複的數字，解決方案無效
        }
    }
    valid[row] = 1;
    return NULL;
}

void *check_col(void *arg) {
    int col = *((int *) arg);
    int count[10] = {0}; // 計數器
    for (int i = 0; i < 9; i++) {
        if (sudoku[i][col] == 0) {
            continue; // 忽略空單元格
        }
        count[sudoku[i][col]]++;
        if (count[sudoku[i][col]] > 1) {
            valid[col] = 0;
            return NULL; // 重複的數字，解決方案無效
        }
    }
    valid[col + 9] = 1;
    return NULL;
}

void *check_region(void *arg) {
    int region_index = *((int *) arg);
    int row_start = (region_index / 3) * 3;
    int col_start = (region_index % 3) * 3;
    int count[10] = {0}; // 計數器
    for (int row = row_start; row < row_start + 3; row++) {
        for (int col = col_start; col < col_start + 3; col++) {
            int val = sudoku[row][col];
            if (val != 0) {
                if (count[val]) {
                    valid[row_start + 9 + col_start/3 + 9] = 0;
                    return NULL;  // 出現重複數字
                }
                count[val] = 1;
            }
        }
    }
    valid[row_start + 9 + col_start/3 + 9] = 1;
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    int rc;
    void *status;
    
    // Create threads to check rows, columns and regions
    for (int i = 0; i < NUM_THREADS; i++){
        thread_args[i] = i%9;
        if (i < 9) {
            rc = pthread_create(&threads[i], NULL, check_row, (void *)&thread_args[i]);
        } else if (9 <= i && i < 18) {
            rc = pthread_create(&threads[i], NULL, check_col, (void *)&thread_args[i]);
        } else {
            rc = pthread_create(&threads[i], NULL, check_region, (void *)&thread_args[i]);
        }

        if (rc) {
            printf("Error: return code from pthread_create() is %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to complete and check their return values
    for (int i = 0; i < NUM_THREADS; i++) {
        rc = pthread_join(threads[i], &status);
        if (rc) {
            printf("Error: return code from pthread_join() is %d\n", rc);
            exit(EXIT_FAILURE);
        }
        
        if (valid[i] == 0){
            printf("Solution is invalid.\n");
            exit(EXIT_SUCCESS);
        }
    }
    printf("Solution is valid.\n");
    pthread_exit(NULL);
}