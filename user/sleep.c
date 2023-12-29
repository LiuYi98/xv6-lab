#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    const char* errorm = "missing arguments\n"; 
    write(1, errorm, strlen(errorm));
  }
  int length = atoi(argv[1]);
  sleep(length); 
  exit(0);
}

