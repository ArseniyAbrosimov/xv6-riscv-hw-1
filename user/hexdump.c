#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int fd;

  if(argc <= 2){
    fprintf(2, "hexdump: wrong args\n");
    exit(1);
  }
  
  if((fd = open(argv[2], O_RDONLY)) < 0){
    fprintf(2, "hexdump: cannot open %s\n", argv[2]);
    exit(1);
  }
  

  int size = atoi(argv[1]);
  
  char *buf;
  if ((buf = malloc(size)) == 0) {
    fprintf(2, "hexdump: kalloc error\n");
    close(fd);
    exit(1);
  }

  int n = 0;
  int m = 0;

  while((size != m) && (n = read(fd, buf + m, size - m)) > 0) {
    m += n;
  }

  if(n < 0){
    fprintf(2, "hexdump: read error\n");
    free(buf);
    close(fd);
    exit(1);
  }

  for (int i = 0; i < size; i++) {
    if (buf[i] < 16)
      printf("0");
    
    printf("%x ", buf[i]);
  }
  printf("\n");

  free(buf);
  close(fd);
  exit(0);
}
