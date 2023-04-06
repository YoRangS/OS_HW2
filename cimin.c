#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <error.h>
// #include <sys/wait.h>

int pipes[2];
char ** return_str;

char * arr_slicing(char * arr, int start, int end){
        int arr_size = end - start;
        char * slice_arr  = (char *)malloc(sizeof(char) * arr_size);
        for(int i = 0; i < arr_size; i++){
                slice_arr[i] = arr[start + i];
        }
        return slice_arr;
}

void
child_proc()
{
        dup2(pipes[1], 1) ;
        //execl("file_path", "args", (char *) 0x0) ;
}

void
parent_proc()
{
        char buf[32] ;
        ssize_t s ;

        close(pipes[1]);

        while ((s = read(pipes[0], buf, 31)) > 0) {
                buf[s+1] = 0x0 ;
                printf("%s/n", buf) ;
        }
}

/*

*/

char * 
reduce(char * tm) {
        // tm <- t
        int s = strlen(tm) - 1 ;

        while (s > 0) {
                for (int i=0; i<strlen(tm)-s; i++) {
                        char * head = arr_slicing(tm, 0, i);
                        char * tail = arr_slicing(tm, i+s, strlen(tm));
                        // o <= p(head + tail)     use exec()
                        pid_t child_pid ;
                        int exit_code ;

                        if ((pipe(pipes)) != 0) {
                                perror("Error");
                                exit(1);
                        }

                        if (child_pid = fork()) {
                                parent_proc();
                        }
                        else {
                                child_proc();
                        }
                        wait(&exit_code);
                        // if (o satisfies with c) {
                        //     return reduce(head+tail)
                        // }

                }
                for (int i=0; i<strlen(tm)-s-1; i++) {
                        char * mid = arr_slicing(tm, i, i+s);
                        // o <= p(mid)     use exec()
                        // if (o satisfies with c) {
                        //     return reduce(mid)
                        // }
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
        memcpy(return_str, args, sizeof(args));
        
        for (int i = 0; i < argc; i++) {
                printf("(%d) %s\n", i, return_str[i]);
        }

        /*
        information[0] = file path of the crashing input
        information[1] = standard error determines.. (i.e. c)
        information[2] = file path to store RCI
        information[3..] = target program (+own argument)
        */

        char* t ; // get crashing input from information[0]
        // minimize(t);

        exit(0);
}