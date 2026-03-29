#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#include "kernel/procinfo.h"

int
main(int argc, char *argv[]) {

  struct procinfo buf[1];
  printf("Test: недостаточный буфер\n");
  int ret = ps_listinfo(buf, 1);
  printf("Результат: %d\n", ret);

  printf("Test: неправильный адрес\n");
  ret = ps_listinfo((struct procinfo*)0xFFFF, 1);
  printf("Результат: %d\n", ret);

  printf("Test: подсчет процессов\n");
  ret = ps_listinfo(0, 1);
  printf("Результат: %d == 3\n", ret);

  struct procinfo buf2[ret];
  printf("Test: буфер размера %d\n", ret);
  ret = ps_listinfo(buf2, ret);
  printf("Результат: %d\n", ret);
}
