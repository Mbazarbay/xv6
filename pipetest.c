#include "fcntl.h"
#include "types.h"
#include "user.h"
//#include "stat.h"

#define MAX_BUF_LEN 512
/* This program will return the number of bytes 
 * currently in a pipe pointed to by fd */
void print_pipe(int v, int g) {
    printf(1, "pipe_count(%d): %d\n", v, pipe_count(v));
    printf(1, "pipe_count(%d): %d\n\n", g, pipe_count(g));
}

int main(int argc, char **argv) {
    int pipefd[2];
    char buf[MAX_BUF_LEN];
    int fd;
    pipe(pipefd);

    printf(1, "Testing pipe_count system call\n");
    printf(1, "Pipe created (read = %d; write = %d)\n", pipefd[0], pipefd[1]); // 0 is read and 1 is write
    print_pipe(pipefd[0], pipefd[1]); // show the pipe is empty

    printf(1, "Writing 'Hello World!' to pipe\n");
    write(pipefd[1], "Hello World!", 12);//The pipe is in between empty and full
    print_pipe(pipefd[0], pipefd[1]);

    printf(1, "Reading 12 bytes from pipe\n");
    read(pipefd[0], buf, 12);
    print_pipe(pipefd[0], pipefd[1]);
    int i;
    printf(1, "Writing 512 bytes to pipe\n");//The pipe is full
    for (i = 0; i < MAX_BUF_LEN; i++) {
        write(pipefd[1], "m", 1);
    }
    print_pipe(pipefd[0], pipefd[1]);

    printf(1, "Getting pipe_count of non-pipe fd\n");
    fd = open("hello.c", O_RDONLY);//The fd is not a valid pipe
    printf(1, "pipe_count(%d): %d\n", pipefd[0], pipe_count(fd));

    exit();
}
