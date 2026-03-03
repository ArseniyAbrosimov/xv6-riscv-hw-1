#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  
  int pid;
  
  pid = fork();
  switch (pid) {
    case -1:
      fprintf(2, "fork error\n");
      exit(1);
    case 0:
      pause(50);
      exit(1);
    default:
      printf("parent pid = %d\n", getpid());
      printf("child pid = %d\n", pid);
      int status;  
      int cpid = wait(&status);
      printf("cpid = %d\n", cpid);
      printf("child status = %d\n", status);
  }
  
  printf("Task b\n");

  pid = fork();
  switch (pid) {
    case -1:
      fprintf(2, "fork error\n");
      exit(1);
    case 0:
      pause(50);
      exit(1);
    default:
      printf("parent pid = %d\n", getpid());
      printf("child pid = %d\n", pid);
      
      if (kill(pid) < 0) {
        fprintf(2, "kill error\n");
        exit(1);       
      }
      int status;  
      int cpid = wait(&status);
      printf("cpid = %d\n", cpid);
      printf("child status = %d\n", status);
  }
  
}
