#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    printf("pipe error");
    exit(1);
  }
  int cpid = fork();
  char b = '0';
  char buf;
  if (cpid == 0) {
    // child read and write
    if (read(pipefd[0], &buf, 1)) {
      printf("%d: received ping\n", getpid());
    }
    write(pipefd[1], &b, 1);
    exit(0);
  } else {
    // parent write and read
    write(pipefd[1], &b, 1);
    if (read(pipefd[0], &buf, 1)) {
      printf("%d: received pong\n", getpid());
    }
    wait(0);
    exit(0);
  }
}

