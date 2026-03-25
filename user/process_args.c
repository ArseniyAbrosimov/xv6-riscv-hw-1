#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  printf("Тест: беспорядочный вывод\n");
  int pid = fork();
  if (pid < 0)
    fprintf(2, "fork fail\n");

  for (int i = 1; i < argc; i++) {
    for (char* c = argv[i]; *c != '\0'; c++) {
      printf("pid %d: argv %d, char '%c'\n", getpid(), i, *c);
    }
  }

  if (pid > 0)
    wait(0);
  else
    exit(0);


  printf("Тест: mutex на вывод\n");
  int mt = mutex();
  pid = fork();
  if (pid < 0)
    fprintf(2, "fork fail\n");
  
  mutex_lock(mt);
  for (int i = 1; i < argc; i++) {
    for (char* c = argv[i]; *c != '\0'; c++) {
      printf("pid %d: argv %d, char '%c'\n", getpid(), i, *c);
    }
  }

  mutex_unlock(mt);

  if (pid > 0)
    wait(0);
  else
    exit(0);
}
