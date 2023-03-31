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

char **
optionManager(int argc, char ** args)
{
        if (0 /*invaild argument*/) {return NULL;}
        
        char ** return_str = (char**) malloc(sizeof(char*) * (argc-4));
        int index = 0;
        for (int i=1; i<argc; i++) {
                if (i==1 || i==3 || i==5) continue;
                return_str[index] = (char*) malloc(sizeof(char) * strlen(args[i]));
                strcpy(return_str, args[i]);
                index++;
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
        char ** information = optionManager(argc, args);
        if (information == NULL) exit(0);
        /*
        information[0] = file path of the crashing input
        information[1] = standard error determines..
        information[2] = file path to store RCI
        information[3..] = target program (+own argument)
        */

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
