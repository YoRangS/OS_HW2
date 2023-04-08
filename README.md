# OS_HW2

## [cimin.c](https://github.com/YoRangS/OS_HW2/blob/master/cimin.c)

### #define and global variable
```
#define CRASH_INPUT_SIZE 4096
#define TIMEOUT_SEC 3

int pipes[2];
char ** return_str;
```
In this program, we assume that a crashing input does not exceed 4096 bytes   
Set CRASH_INPUT_SIZE to 4096.   
We set TIMEOUT_SEC to 3 to detect infinite loops when we turn the balance.   


### 'void child_proc()'
creats a child process named child_proc() and passes the output of the child process
to the parent process through a pipe.

1. A pipe is created to communicate between the parent process and the child process. The pipes array stores the file descriptors of the pipe.
2. The fork() is called to create a child process. The child process uses the execl() to execute an external program.
3. The standard output of the child process is redirected to the write and of the pipe(pipes[1]).
4. The execl() is used to execute the external program lacated at the specified path(file_path).
5. When the external program execution is complete, the child process terminates. The pipe is automatically closed.



### 'int parent_proc()' 
that is used to read the output of a child process.
It also sets a timeout value and terminates the child process if it exceeds the timeout.

'buf' : a character array with a size of 128.   
's' : a signed integer of type 'ssize_t'.   
'tv' : a struct of type 'timeval'.   
'readfds' : a set of file descriptors for reading.   
'ret' : a signed integer of type 'int'.   
'exit_code' : an integer that stores the exit status of the    child process.
'pipes' : an array of two integers used for communication    between parent and child process.   
'child_pid' : a process identifier of the child process.

1. Close the write end of the pipe.
2. Set the timeout value using struct timeval.
3. Create a file descriptor set using fd_set and add the read end of the pipe to it.
4. Use select() to wait for the file descriptor to be ready for reading.
5. If select() returns -1, print an error message and exit.
6. If select() times out, print a message, kill the child process using SIGTERM, wait for it to exit, and then exit the parent process with a status code of 1.
7. If the file descriptor is ready for reading, use read() to read the data into buf.
8. Check if buf contains a specific substring using strstr().
9. Return the result of the substring search.




### 'int ProgramExecution(char * testInput)'
exetues an external program with a specified input and returns the result.

1. A pipe is created using the pipe() system call to enable communication between the parent and child processes. The pipes array is used to store the file descriptors for the pipe.
2. The fork() system call is used to create a child process. The parent process continues executing while the child process executes a different code path.
3. The child process executes child_proc() function which redirects its standard output to the write end of the pipe using dup2() function and executes an external program using execl() function.
4. The parent process executes parent_proc() function which reads the output of the external program from the read end of the pipe using select() and read() functions and checks if it contains a specific substring.
5. The parent process waits for the child process to complete using the wait() system call and gets its exit code.





### 'char * reduce()'
takes a string as an argument, cuts out parts of the string to check the result of program execution, and if the program execution result does not satisfy a certain condition, it reassembles the string and repeats the process of checking the program execution result until it finds a satisfying string to return.

1. Save the length of the input string t in s.
2. Cut the string t in the range from s to 1, and check the program execution result.
3. If the program execution result does not satisfy certain conditions, pass the cut part of the string as an argument to the reduce() function to perate recursively. This process continues until all parts of the string are checked.
4. After cutting the string, divide it into head and tail, allocate dynamic memory, and concatenate head and tail to create a new string called test_input.
5. Pass the created test_input as an argument to the ProgramExecution() to check the program execution result.
6. If the program execution result does not satisfy certain conditions, call the reduce() recursively.
7. When the recursively called reduce() returns, if the string is a part the previously cut string, divide the cut string into head and tail and check the program execution result. 
8. If no satisfying string is found after checking all parts of the string, return the string t itself.



### 'char * minimize(char * t)'
minimizes the given string by calling the reduce() function internally and returns the result of reduce() as its own return value.



### 'void create_reduced(char * t)'

1. Receives a string t as an argument.
2. Calls the popen function to execute the external program specified by return_str[6]. It returns a FILE pointer connected to the external program using the "w" option.
3. If the FILE pointer is NULL, i.e., if the external program execution fails, it prints an error message and terminates the program.
4. Uses the fprintf function to output the t string to the stream pointed to by the FILE pointer.
5. Calls the pclose function to close the connection between the file pointer and the external program.


### 'int checkInvaildArgument(int argc, char ** args)'
performs the role of verifying whether all given arguments are valid options. Therefore, it is one of the essential functions to ensure program stability.

1. Receives argc and args array as arguments.
2. Declares check array and initializes all values to 0.
3. Accesses the args array using a for loop, and if the argument is an option, changes the corresponding value in the check array to 1.
4. Finally, if all values in the check array are 1, it returns 1, otherwise, it returns 0.



### 'int main()'

1. Receives arguments argc and args as input.
2. Calls the checkInvaildArgument function to verify if the arguments are valid options. If an invalid option is found, it prints the error message "Invalid Argument" and exits the program.
3. Dynamically allocates a char pointer array return_str of size argc. Copies the values from args to return_str using the memcpy function.
4. Uses a for loop to dynamically allocate memory for each index of return_str and copies the string from args using the strcpy function.
5. Prints the values in return_str array using a for loop.
6. Declares a char array buff of size CRASH_INPUT_SIZE.
7. Executes an external program specified by args[2] using the popen function. Returns a FILE pointer that is connected to the external program with the "r" option.
8. If the fp file pointer is NULL, indicating a failed execution of the external program, it prints an error message and exits the program.
9. Reads the string output from the external program using the fgets function into the buff array.
10. Closes the connection between the file pointer and the external program using the pclose function.
11. Uses the minimize function to shorten the buff string and creates a reduced string using the create_reduced function with the shortened string as an argument.
12. Frees the dynamically allocated memory in the return_str array.
13. Exits the program.



## [Makefile](https://github.com/YoRangS/OS_HW2/blob/master/Makefile)

### make all

```
all: $(TARGET)

$(TARGET): cimin.o
	$(CC) -o $(TARGET) cimin.o

cimin.o: cimin.c
	$(CC) $(CFLAGS) cimin.c
```

First, do 'make all' then create 'cimin.o' and 'cimin'.   
To execute this code.
```
gcc -c cimin.c
gcc -o cimin cimin.o
```

### make _reduced

