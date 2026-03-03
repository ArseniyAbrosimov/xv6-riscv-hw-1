#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUFSIZE 128

char buf[BUFSIZE];
int pos = 0;

void flush(int fd) {
  int sent = 0;
  while (sent != pos) {
    int res = write(fd, buf + sent, pos - sent);
    if (res < 0) {
      fprintf(2, "error write argv");
      exit(1);
    }
    sent += res;
  }
  pos = 0;
}

void send(int fd, char* str) {
  if (strlen(str) + 1 > BUFSIZE - pos) {
    flush(fd);
  }
  memmove(buf + pos, str, strlen(str));
  pos += strlen(str) + 1;
  buf[pos-1] = '\n';
}

int main(int argc, char *argv[]) {

  int pid;
  int pipefd[2];

  if (pipe(pipefd) < 0) {
    fprintf(2, "pipe error\n");
    exit(1);
  }

  pid = fork();
  switch (pid) {
    case -1:
      fprintf(2, "fork error\n");
      exit(1);
    case 0:
      if (close(0) < 0) {
        fprintf(2, "close error\n");
        exit(1);
      }
      if (dup(pipefd[0]) < 0) {
        fprintf(2, "dup error\n");
        exit(1);
      }
      if (close(pipefd[0]) < 0) {
        fprintf(2, "close pipefd 0 error\n");
        exit(1);
      }
      if (close(pipefd[1]) < 0) {
        fprintf(2, "close pipefd 1 error\n");
        exit(1);
      }

      char *args[] = {"/wc", 0};
      exec("/wc", args);
      fprintf(2, "exec error\n");
      exit(1);
    default:
      printf("parent pid = %d\n", getpid());
      printf("child pid = %d\n", pid);

      int status;
      if (close(pipefd[0]) < 0) {
        fprintf(2, "close pipefd 0 error\n");
        exit(1);
      }

      for (int i = 1; i < argc; i++) {
        send(pipefd[1], argv[i]);
      }
      flush(pipefd[1]);

      if (close(pipefd[1]) < 0) {
        fprintf(2, "close pipefd 1 error\n");
        exit(1);
      }

      int cpid = wait(&status);
      printf("cpid = %d\n", cpid);
      printf("child status = %d\n", status);
  }
  
 
}
