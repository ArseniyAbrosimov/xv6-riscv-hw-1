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
    struct procinfo buf[lim];
  
    res = ps_listinfo(buf, lim);
    if (res == COPYERR){
      fprintf(2, "Ошибка записи в буфер");
      exit(-1);
    } else if (res == BUFSIZEERR)
      lim *= 2;
    else {
      printf("Таблица процессов\n");
      printf("pid\tname\tstate\tparent\n");
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
        printf("%d\n", p->parent);
      }
      break;
    }
  }
}
