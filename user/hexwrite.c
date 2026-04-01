#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int decode_char(char c) {
  if (c <= 'F' && c >= 'A') {
    return  c - 'A' + 10;
  } else if (c <= 'f' && c >= 'a') {
    return  c - 'a' + 10;
  } else if (c <= '9' && c >= '0') {
    return  c - '0';
  }

  return -1;
}

int decode(char* byte, char* dst) {
  int h = decode_char(byte[0]);
  int l = decode_char(byte[1]);
  if (h == -1 || l == -1) {
    return -1;
  }
  *dst = h * 16 + l;
  return 0;
}

int
main(int argc, char *argv[])
{
  int fd;

  if(argc <= 2){
    fprintf(2, "hexwrite: wrong args\n");
    exit(1);
  }
  
  if((fd = open(argv[2], O_WRONLY)) < 0){
    fprintf(2, "hexwrite: cannot open %s\n", argv[2]);
    exit(1);
  }
  

  int size = strlen(argv[1]);
  int n = 0;
  int m = 0;

  char* buf;

  if ((buf = malloc(size / 2)) == 0) {
    fprintf(2, "hexwrite: kalloc error\n");
    close(fd);
    exit(1);
  }

  int l;
  for (int i = 0; i < size; i += 2) {
    l = decode(argv[1] + i, buf + i/2);
    if (l == -1) {
      fprintf(2, "hexwrite: parse error\n");
      free(buf);
      close(fd);
      exit(1);
    }
  }
  
  while((m != size/2) && (n = write(fd, buf + m, size/2 - m)) > 0) { 
    m += n;
  }

  if (n < 0) {
    fprintf(2, "hexwrite: write error\n");
    free(buf);
    close(fd);
    exit(1);
  }

  free(buf);
  close(fd);
}
