#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <error.h>
// #include <sys/wait.h>

#define CRASH_INPUT_SIZE 4096

int pipes[2];
char ** return_str;

void
child_proc()
{
        dup2(pipes[1], 1) ;
        execl("file_path", "args", (char *) 0x0) ;
}

int
parent_proc()
{
        char buf[128] ;
        ssize_t s ;

        close(pipes[1]);

        if ((s = read(pipes[0], buf, 127)) > 0) {
                perror("Failed to read output");
        } else {
                buf[s+1] = 0x0 ;
        }
        return strstr(return_str[4], buf) != NULL;
}

/*
o <- p(head+tail)
In child, 
        dup2(stdout);
        execl("args[2]", head+tail, (char *) 0x0);
        then stdout receive execl's result and give parent
In parent,
        result = receive execl's result
        return result
*/

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

void
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

void minimize(char * t) {
        return reduce(t);
}

int
main(int argc, char ** args)
{
        return_str = (char**)malloc(argc * sizeof(char*));
        memcpy(return_str, args, argc * sizeof(char *));

        for (int i = 0; i < argc; i++) {
                return_str[i] = (char*)malloc(strlen(args[i]) + 1);
                strcpy(return_str[i], args[i]);
        }
        
        for (int i = 0; i < argc; i++) {
                printf("(%d) %s\n", i, return_str[i]);
        }

        char* file_path = "../OperatingSystem";
        char* program_path;
        if(strcmp(args[7], "balance") == 0) {
                program_path = "/balance/testcases/fail";
        } else if(strcmp(args[7], "jsondump") == 0) {
                program_path = "/jsmn/testcases/crash.json";
        } else if(strcmp(args[7], "xmllint") == 0) {
                program_path = "/libxml2/testcases/crash.xml";
        } else if(strcmp(args[7], "test_pngfix") == 0) {
                program_path = "/libpng/crash.png";
        }
        strcat(file_path, program_path);

        char buff[CRASH_INPUT_SIZE];
        FILE *fp;
        fp = popen(file_path, "r");
        if(NULL == fp) {
                perror("Failed popen");
                return -1;
        }
        fgets(buff, CRASH_INPUT_SIZE, fp);

        pclose(fp);

        minimize(buff);

        free(return_str);

        exit(0);
}

/*
        args[2] = file path of the crashing input
        args[4] = standard error determines.. (i.e. c)
        args[6] = file path to store RCI
        args[7..] = target program (+own argument)
*/