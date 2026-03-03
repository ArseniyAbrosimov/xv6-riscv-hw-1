#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 1024

char buf[BUFSIZE];
int pos = 0;

void flush(int fd) {
  int sent = 0;
  while (sent != pos) {
    int res = write(fd, buf + sent, pos - sent);
    if (res < 0) {
      perror("error write argv");
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

  pid_t pid;
  int pipefd[2];

  if (pipe(pipefd) < 0) {
    perror("pipe error");
    exit(1);
  }

  pid = fork();
  switch (pid) {
    case -1:
      perror("fork error");
      exit(1);
    case 0:
      if (close(0) < 0) {
        perror("close error");
        exit(1);
      }
      if (dup(pipefd[0]) < 0) {
        perror("dup error");
        exit(1);
      }
      if (close(pipefd[0]) < 0) {
        perror("close pipefd 0 error");
        exit(1);
      }
      if (close(pipefd[1]) < 0) {
        perror("close pipefd 1 error");
        exit(1);
      }

      char* read_buf[BUFSIZE];
      int n;
      while ((n = read(0, read_buf, sizeof(read_buf))) > 0) {
        int sent = 0;
        while (sent != n) {
          int res = write(1, read_buf + sent, n - sent);
          if (res < 0) {
            perror("error write argv");
            exit(1);
          }
          sent += res;
        }
      }
      sleep(150);
      exit(EXIT_SUCCESS);
    default:
      printf("child pid = %jd\n", (intmax_t) pid);
      if (close(pipefd[0]) < 0) {
        perror("close pipefd 0 error");
        exit(1);
      }

      for (int i = 1; i < argc; i++) {
        send(pipefd[1], argv[i]);
      }
      flush(pipefd[1]);

      if (close(pipefd[1]) < 0) {
        perror("close pipefd 1 error");
        exit(1);
      }
      int status;
      wait(&status);
      printf("child status = %i\n", WEXITSTATUS(status));
  }
  
 
}
