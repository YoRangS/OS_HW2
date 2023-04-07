#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <error.h>
#include <sys/wait.h>
#include <signal.h>

#define CRASH_INPUT_SIZE 4096
#define TIMEOUT_SEC 3


int pipes[2];
char ** return_str;

void
child_proc()
{
        dup2(pipes[1], 1) ;
        execl("file_path", "args", (char *) 0x0) ;
}

int parent_proc()
{
        char buf[128] ;
        ssize_t s ;

        close(pipes[1]);

        // timeout
        struct timeval tv;
        tv.tv_sec = TIMEOUT_SEC;
        tv.tv_usec = 0;

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(pipes[0], &readfds);

        pid_t child_pid ;
        int exit_code ;

        // Set timeout to use select function
        int ret = select(pipes[0] + 1, &readfds, NULL, NULL, &tv);
        if (ret == -1) {
            perror("select() failed");
            exit(1);
        } else if (ret == 0) {  // timeout
            printf("Execution timed out\n");
            kill(child_pid, SIGTERM);  // Kill child process
            wait(&exit_code);
            exit(1);
        } else {  
            if ((s = read(pipes[0], buf, 127)) > 0) {
                    perror("Failed to read output");
            } else {
                    buf[s+1] = 0x0 ;
            }
            return strstr(return_str[4], buf) != NULL;
        }
}

int ProgramExecution(char * testInput) {
        pid_t child_pid ;
        int exit_code ;

        if ((pipe(pipes)) != 0) {
                perror("Failed to read pipe");
                exit(1);
        }

        if (child_pid = fork()) {
                parent_proc();
        }
        else {
                child_proc();
        }
        wait(&exit_code);
}

char *
reduce(char * t) {
        char * tm = t;
        int s = strlen(tm) - 1 ;

        while (s > 0) {
                for (int i=0; i<strlen(tm)-s; i++) {
                        char * head = (char*)malloc((i+1)*sizeof(char));
                        strncpy(head, tm, i);
                        head[i] = '\0';
                        char * tail = (char*)malloc((i+1)*sizeof(char));
                        strncpy(tail, tm+i+s, strlen(tm)-(i+s));
                        tail[strlen(tm)-(i+s)] = '\0';

                        int len = snprintf(NULL, 0, "%s%s", head, tail);
                        char * test_input = (char*) malloc(len + 1);
                        snprintf(test_input, len+1, "%s%s", head, tail);

                        if (ProgramExecution(test_input)) {
                                return reduce(test_input);
                        }
                        free(head); free(tail); free(test_input);
                }
                for (int i=0; i<strlen(tm)-s-1; i++) {
                        char * mid = (char*)malloc((i+1)*sizeof(char));
                        strncpy(mid, tm+i, s);
                        mid[i] = '\0';

                        if (ProgramExecution(mid)) {
                                return reduce(mid);
                        }
                }
                s = s - 1 ;
        }
        return tm ;
}

char* minimize(char * t) {
        return reduce(t);
}

void create_reduced(char* t){
        FILE* file = popen(return_str[6], "w");
        if (file == NULL) {
                perror("Failed to write");
                exit(0);
        }

        fprintf(file, "%s", t);
        pclose(file);
}

int checkInvaildArgument(int argc, char ** args) {
        int check[3] = {0,};
        for(int i = 0; i < argc; i++) {
                char* option = args[i];
                if(i < 6 && i % 2 == 1) {
                        if(strcmp(option, "-i") == 0) check[0] = 1;
                        else if(strcmp(option, "-m") == 0) check[1] = 1;
                        else if(strcmp(option, "-o") == 0) check[2] = 1;
                }
        }
        if (check[0] == 1 && check[1] == 1 && check[2] == 1) return 1;
        else return 0;
}

int
main(int argc, char ** args)
{
        if(!checkInvaildArgument(argc, args)) {
                perror("Invaild Argument");
                exit(0);
        }

        return_str = (char**)malloc(argc * sizeof(char*));
        memcpy(return_str, args, argc * sizeof(char *));

        for (int i = 0; i < argc; i++) {
                return_str[i] = (char*)malloc(strlen(args[i]) + 1);
                strcpy(return_str[i], args[i]);
        }
        
        for (int i = 0; i < argc; i++) {
                printf("(%d) %s\n", i, return_str[i]);
        }

        char buff[CRASH_INPUT_SIZE];
        FILE *fp;
        fp = popen(args[2], "r");
        if(NULL == fp) {
                perror("Failed popen");
                return -1;
        }
        fgets(buff, CRASH_INPUT_SIZE, fp);

        pclose(fp);

        create_reduced(minimize(buff));

        free(return_str);

        exit(0);
}