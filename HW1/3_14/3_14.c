#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(){
    int input;
    scanf("%d", &input);

    if (input <= 0){
        printf("Input must be > 0\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1){
        printf("Fork Failed\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0){
    printf("%d", input);
    while (input != 1){
        if (input % 2 == 0){
            input = input / 2;
        }
        else{
            input = input * 3 + 1;
        }
        printf(", %d", input);
    }
    printf(".\n");
    exit(EXIT_SUCCESS);
    }
    else{
        pid_t wpid = waitpid(pid, NULL, 0);
        printf("Fork end\n");
        exit(EXIT_SUCCESS);
    }
    return 0;
}