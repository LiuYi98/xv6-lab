#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

void check_name(const char *filename, const char *path, const char *pattern) {
  if (strcmp(filename, pattern) == 0) {
    printf("%s\n", path);
  };
}

void find(const char *path, const char *pattern) {

  int fd;
  struct stat st;
  struct dirent de;

  // use a buffer to store path, p points to the end of (the path plus '/') in buf
  uint BUF_SIZE = 512;
  char buf[BUF_SIZE + 1];
  char *p;

  uint path_length;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    exit(1);
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    exit(1);
  }

  
  switch (st.type) {
  case T_DEVICE:
  case T_FILE:
    check_name(path, path, pattern);      
    break;
  case T_DIR:
    // put "{path}/" into buf
    path_length = strlen(path);

    memmove(buf, path, path_length);
    buf[path_length] = '/';
    p = buf + path_length + 1;

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0 || strcmp(de.name, "") == 0 ||
        strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
        continue;
      }
      if (path_length + 1 + strlen(de.name) > BUF_SIZE) {
        fprintf(2, "path length too long");
        continue;
      }
      // be sure to move length of DIRSIZ(the max size of a filename can be), not strlen(de.name),
      // to overwrite the previous filename in buf, like overwriting 'longer_filename' with 'filename'
      memmove(p, de.name, DIRSIZ);
      
      buf[BUF_SIZE] = '\0';
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }

      if (st.type == T_DIR) {
        find(buf, pattern);
      } else  {
        check_name(de.name, buf, pattern);
      }
    }
    break;
  default:
    break;
  }
  close(fd);
}

int main(int argc, char *argv[]) {

  if (argc < 3) {
    printf("usage: find [dest] [pattern]\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}
