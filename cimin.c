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

char * arr_slicing(char * arr, int start, int end){
        int arr_size = end - start;
        char * slice_arr  = (char *)malloc(sizeof(char) * arr_size);
        for(int i = 0; i < arr_size; i++){
                slice_arr[i] = arr[start + i];
        }
        return slice_arr;
}

void
optionManager(int argc, char ** args)
{
        char * file_path ;
        char * stderr_str ;
        char * store_RCI ;
        for (int i=1; i<argc; i++) {
                if (args[i][0] == '-') {
                        if (args[i][1] == 'i') {
                                // printf("File path of the crashing Input : %s\n",args[i+1]);
                                file_path = args[i+1] ;
                        } else if (args[i][1] == 'm') {
                                // printf("standard error String : %s\n",args[i+1]);
                                stderr_str = args[i+1] ;
                        } else if (args[i][1] == 'o') {
                                // printf("File path to store the RCI : %s\n",args[i+1]);
                                store_RCI = args[i+1] ;
                        } else { printf("Invaild option\n"); }
                }
                if (i == argc - 1) {
                    // save file name (p)
                }
        }
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

char * 
reduce(char * tm) {
        // tm <- t
        int s = strlen(tm) - 1 ;

        while (s > 0) {
                for (int i=0; i<strlen(tm)-s-1; i++) {
                        char * head = arr_slicing(tm, 0, i+1);
                        char * tail = arr_slicing(tm, i+s+1, strlen(tm));
                    // o <= p(head + tail)     use exec()
                    // if (o satisfies with c) {
                    //     return reduce(head+tail)
                    // }

                }
                for (int i=0; i<strlen(tm)-s-1; i++) {
                        char * mid = arr_slicing(tm, i, i+s+1);
                    // o <= p(mid)     use exec()
                    // if (o satisfies with c) {
                    //     return reduce(mid)
                    // }
                }
                s = s - 1 ;
        }
        return tm ;
}

int
main(int argc, char ** args)
{
        optionManager(argc, args);

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

        exit(0);
}
