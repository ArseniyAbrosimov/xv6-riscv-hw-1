#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define PGSIZE 4096
#define PTE_A (1L << 6)
#define PTE_D (1L << 7)


int global = 0;

void
check(int *stack_addr, int *global_addr, int *stack_arr_addr, int *heap_arr_addr)
{
  if (check_flags(stack_addr, sizeof(int), PTE_A | PTE_D))
    printf("Флаг у переменной на стеке поменялся\n");
  else
    printf("Флаг не поменялся\n");
  if (check_flags(global_addr, sizeof(int), PTE_A | PTE_D))
    printf("Флаг у глобальной переменной поменялся\n");
  else
    printf("Флаг не поменялся\n");
  if (check_flags(stack_arr_addr, sizeof(stack_arr_addr), PTE_A | PTE_D))
    printf("Флаг у массива на стеке поменялся\n");
  else
    printf("Флаг не поменялся\n");
  if (check_flags(heap_arr_addr, sizeof(heap_arr_addr), PTE_A | PTE_D))
    printf("Флаг у массива на куче поменялся\n");
  else
    printf("Флаг не поменялся\n");
}

int
main(int argc, char *argv[])
{
  
  char str1[] = "Таблица в начале\n";
  char str2[] = "Таблица после выделения\n";
  char str3[] = "Таблица после очистки флагов\n";
  char str4[] = "Тест: чтение, PTE_A\n";
  char str5[] = "Тест: запись, PTE_D\n";
  char str6[] = "Тест: освобождение памяти\n";

  write(0, str1, sizeof(str1));
  print_page();
  
  int stack = 0;
  int stack_arr[10];
  int *heap_arr;

  heap_arr = (int*)malloc(PGSIZE * 2 * sizeof(int));


  write(0, str2, sizeof(str2));
  print_page();

  clear_flags(&stack, sizeof(int), PTE_A | PTE_D);
  clear_flags(&global, sizeof(int), PTE_A | PTE_D);
  clear_flags(stack_arr, 10 * sizeof(int), PTE_A | PTE_D);
  clear_flags(heap_arr, PGSIZE * 2 * sizeof(int), PTE_A | PTE_D);

  write(0, str3, sizeof(str3));
 
  print_page();
  write(0, str4, sizeof(str4));

  volatile int test;

  test = stack;
  test = global;
  test = stack_arr[0];
  test = heap_arr[0];

  if (check_flags(&stack, sizeof(int), PTE_A))
    write(0, "Флаг у переменной на стеке поменялся\n", 69);
  else
    write(0, "Флаг не поменялся\n", 34);
  if (check_flags(&global, sizeof(int), PTE_A))
    write(0, "Флаг у глобальной переменной поменялся\n", 74);
  else
    write(0, "Флаг не поменялся\n", 34);
  if (check_flags(stack_arr, 10 * sizeof(int), PTE_A))
    write(0, "Флаг у массива на стеке поменялся\n", 63);
  else
    write(0, "Флаг не поменялся\n", 34);
  if (check_flags(heap_arr, PGSIZE * 2 * sizeof(int), PTE_A))
    write(0, "Флаг у массива на куче поменялся\n", 61);
  else
    write(0, "Флаг не поменялся\n", 34);


  
  clear_flags(&stack, sizeof(int), PTE_A);
  clear_flags(&global, sizeof(int), PTE_A);
  clear_flags(stack_arr, 10 * sizeof(int), PTE_A);
  clear_flags(heap_arr, PGSIZE * 2 * sizeof(int), PTE_A);

  
  write(0, str5, sizeof(str5));
  

  stack = test;
  global = 20;
  stack_arr[0] = 30;
  heap_arr[0] = 40;

  if (check_flags(&stack, sizeof(int), PTE_D))
    write(0, "Флаг у переменной на стеке поменялся\n", 69);
  else
    write(0, "Флаг не поменялся\n", 34);
  if (check_flags(&global, sizeof(int), PTE_D))
    write(0, "Флаг у глобальной переменной поменялся\n", 74);
  else
    write(0, "Флаг не поменялся\n", 34);
  if (check_flags(stack_arr, 10 * sizeof(int), PTE_D))
    write(0, "Флаг у массива на стеке поменялся\n", 63);
  else
    write(0, "Флаг не поменялся\n", 34);
  if (check_flags(heap_arr, PGSIZE * 2 * sizeof(int), PTE_D))
    write(0, "Флаг у массива на куче поменялся\n", 62);
  else
    write(0, "Флаг не поменялся\n", 34);

  
  clear_flags(&stack, sizeof(int), PTE_A | PTE_D);
  clear_flags(&global, sizeof(int), PTE_A | PTE_D);
  clear_flags(stack_arr, 10 * sizeof(int), PTE_A | PTE_D);
  clear_flags(heap_arr, PGSIZE * 2 * sizeof(int), PTE_A | PTE_D);

  write(0, str6, sizeof(str6));

  free(heap_arr);


   if (check_flags(&stack, sizeof(int), PTE_A | PTE_D))
    write(0, "Флаг у переменной на стеке поменялся\n", 69);
  else
    write(0, "Флаг не поменялся\n", 34);
  if (check_flags(&global, sizeof(int), PTE_D | PTE_A))
    write(0, "Флаг у глобальной переменной поменялся\n", 74);
  else
    write(0, "Флаг не поменялся\n", 34);
  if (check_flags(stack_arr, 10 * sizeof(int), PTE_D | PTE_A))
    write(0, "Флаг у массива на стеке поменялся\n", 63);
  else
    write(0, "Флаг не поменялся\n", 34);
  if (check_flags(heap_arr, PGSIZE * 2 * sizeof(int), PTE_D | PTE_A))
    write(0, "Флаг у массива на куче поменялся\n", 62);
  else
    write(0, "Флаг не поменялся\n", 34);


  exit(0);

}
