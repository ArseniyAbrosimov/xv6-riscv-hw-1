#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#include "kernel/procinfo.h"

#define COPYERR -3
#define BUFSIZEERR -2

int
main(int argc, char *argv[]) {
  
  int lim = 1;
  int res;

  for (;;) {
    struct procinfo *buf = malloc(lim * sizeof(struct procinfo));

    if (buf == 0) {
      fprintf(2, "Ошибка malloc");
      exit(-1);
    }

    res = ps_listinfo(buf, lim);
    if (res == COPYERR){
      fprintf(2, "Ошибка записи в буфер");
      exit(-1);
    } else if (res == BUFSIZEERR)
      lim *= 2;
    else {
      printf("Таблица процессов\n");
      printf("pid\tname\tstate\t\tparent\tpname\n");
      struct procinfo *p;
      for (p = buf; p < &buf[res]; p++) {
        printf("%d\t", p->pid);
        printf("%s\t", p->name);
        switch(p->state) {
          case MY_USED: 
            printf("USED    \t");
            break;
          case MY_SLEEPING:
            printf("SLEEPING\t");
            break;
          case MY_RUNNABLE:
            printf("RUNNABLE\t");
            break;
          case MY_RUNNING:
            printf("RUNNING \t");
            break;
          case MY_ZOMBIE:
            printf("ZOMBIE  \t");
            break;
          default:
        }
        if (p->parent != 0) {
          printf("%d\t", p->parent);
          struct procinfo *parent;
          for (parent = buf; parent < &buf[res]; parent++) {
            if (parent->pid == p->parent) {
              printf("%s\n", parent->name);
            }
          }
        } else {
          printf("no\t");
          printf("no\n");
        }
      }
      free(buf);
      break;
    }
    free(buf);
  }
}
