#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define READ_BUFSIZE 1024
#define BUFSIZE 64
#define TOK_DELIM " \t\r\n"
#define DEFAULT -1

enum command_id{
    CMD_CD,
    CMD_LS,
    CMD_PWD,
    CMD_HIS,
    CMD_EXIT
};

typedef struct command_set {
    char _ls[2][3];
    char _pwd[4];
    char _cd[6];
}command_set;

command_set c_set = {
    ._ls = "ls",
    ._pwd ="pwd",
    ._cd = "cd .."
};

char * cmd_line_read (void);
char ** cmd_get_arg(char *);
int cmd_analysis(char *,int *);
void shell_cd_command(char **);
void shell_default_command(char**);
void push_command_log(char ***,char * ,int *);
void shell_history_command(char **,int);


char * cmd_line_read(void){
    char * buffer = (char *)malloc(sizeof(char)*READ_BUFSIZE);
    fgets(buffer,READ_BUFSIZE,stdin);
    return buffer;
}

char ** cmd_get_arg(char * cmd_line){
    int bufsize = BUFSIZE, pos = 0;
    char **tokens = (char*) malloc(bufsize * sizeof(char*));
    char *arg;

    arg = strtok(cmd_line,TOK_DELIM);
    while(arg != NULL){
        tokens[pos++] = arg;
        arg = strtok(NULL,TOK_DELIM);
    }
    return tokens;

}
int cmd_analysis(char * cmd_line,int *stat){
    if (strcmp(cmd_line, "cd") == 0){
        stat[CMD_CD]++;
        return CMD_CD;
    } else if (strcmp(cmd_line, "history") == 0){
        return CMD_HIS;
    } else if (strcmp(cmd_line, "pwd") == 0){
        stat[CMD_PWD]++;
    } else if (strcmp(cmd_line, "ls") == 0){
        stat[CMD_LS]++;
    } else if (strcmp(cmd_line, "exit") == 0){
        return CMD_EXIT;
    }
    return DEFAULT;
}
void push_command_log(char *** history_log,char * cmd_line, int *count){
    *history_log = (char**) realloc(*history_log, (* count + 1 )*sizeof(char*));
    (*history_log)[*count] = (char*)malloc(strlen(cmd_line)+1);
    strcpy((*history_log)[*count], cmd_line);
    (*count)++;
}
void shell_cd_command(char** args){
    char cd_path[BUFSIZE];

    if (getcwd(cd_path, sizeof(cd_path)) != NULL) {
        printf("Current working directory: %s", getcwd(cd_path, sizeof(cd_path)));
        chdir(args[1]);
        printf("%s\n", getcwd(cd_path, sizeof(cd_path)));
    } else {
        perror("error");
    }
}
void shell_history_command(char ** log,int log_size){
    if(log_size == 0){
        printf("History is empty\n");
        return ;
    }
    for(int i = 0 ;i < log_size ; i++ ){
        printf("%d ",i);
        for (int y =0 ;y < strlen(log[i]); y++ ){
            printf("%c",log[i][y]);
        }
        printf("\n");
    }
}
void shell_default_command(char** args){
    pid_t pid = fork();
    if (pid < 0){
        perror("Error forking child process");
    }
    else if (pid == 0){
        if (execvp(args[0], args) == -1){
            perror("Error executing command");
        }
        exit(EXIT_FAILURE);
    }
    else{
        wait(NULL);
    }
}

int main(void){
    int stat[]= {0,0,0};
    int count_cmd = 0;
    char **history_log = (char **) malloc(BUFSIZE * sizeof(char *));

    while(1){
        char *cmd_line = NULL, * cmd = NULL; 
        char ** args = NULL ,** args_his = NULL;
        int status = -1 ;
        
        printf("shell_>");
        cmd_line = cmd_line_read();
        args = cmd_get_arg(cmd_line);
        cmd = args[0];
        status = cmd_analysis(cmd_line,stat);
        push_command_log(&history_log, cmd_line, &count_cmd);
        switch (status) {
            case CMD_CD:
                    shell_cd_command(args);
                break;
            case CMD_EXIT:
                    free(cmd_line);
                    free(args);
                    return 0;
                break;
            case CMD_HIS:
                    shell_history_command(history_log,count_cmd);
                break;
            default:
                    shell_default_command(args);
                break;
        }

        fflush(stdout);
        free(cmd_line);
        free(args);
        free(args_his);
    }
    return 0 ;
}