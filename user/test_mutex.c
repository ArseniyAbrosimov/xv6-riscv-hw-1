#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char **argv)
{
  printf("Read/write mutex\n");
  int mt = mutex();
  char buf[10];
  int read_res = read(mt, buf, 10);
  int write_res = read(mt, buf, 10);
  printf("Вывод: read = %d write = %d == -1\n", read_res, write_res);
  printf("\n");

  printf("Тест: закрытие захваченного мьютекса тем же процессом\n");
  int close_res;
  if((close_res = close(mt)) < 0) {
    fprintf(2, "ошибка закрытия\n");
    return -1;
  }
  printf("Вывод: %d == 0\n", close_res);
  printf("\n");

  printf("Тест: закрытие захваченного мьютекса другим процессом\n");
  int mt2 = mutex();
  int pid = fork();

  if (pid < 0) {
    fprintf(2, "ошибка fork\n");
    return -1;
  } else if (pid == 0) {
    mutex_lock(mt2);
    pause(20);
    close(mt2);
    exit(0);
  } else {
    pause(10);
    //дождемся чтобы мьютекс захватили
    int close_res2 = close(mt2);
    printf("Вывод: %d == 0\n", close_res2);
    wait(0);
    printf("Закрытие только после завершения дочернего процесса\n");
  }
  printf("\n");

  printf("Тест: завершение процесса при наличии незакрытых мьютексов\n");
  pid = fork();
  if (pid < 0) {
    fprintf(2, "ошибка fork\n");
    return -1;
  } else if (pid == 0) {
    int mt3 = mutex();
    mutex_lock(mt3);
    exit(0);
  } else {
    wait(0);
    printf("Вывелось сообщение о закрытии захваченного мьютекса без close\n");
  }
  printf("\n");

  printf("Тест: попытка освобождения захваченного мьютекса другим процессом\n");
  int mt4 = mutex();
  pid = fork();

  if (pid < 0) {
    fprintf(2, "ошибка fork\n");
    return -1;
  } else if (pid == 0) {
    mutex_lock(mt4);
    pause(20);
    close(mt4);
    exit(0);
  } else {
    pause(10);
    //дождемся чтобы мьютекс захватили
    int unlock_res = mutex_unlock(mt4);
    printf("Вывод: %d == -1\n", unlock_res);
    wait(0);
  }
}
