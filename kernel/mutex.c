#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"
#include "mutex.h"

int
mutexalloc(struct file **file)
{
  struct mutex *mutex;

  mutex = 0;
  *file = 0;
  if((*file = filealloc()) == 0) {
    printf("file cannot be allocated\n");
    return -1;
  }
  if((mutex = (struct mutex*)kalloc()) == 0) {
    fileclose(*file);
    printf("mautex cannot be allocated\n");
    return -1;
  }
  
  initsleeplock(&mutex->lock, "mutex");

  (*file)->type = FD_MUTEX;
  (*file)->readable = 0;
  (*file)->writable = 0;
  (*file)->mutex = mutex;
  printf("mutex has been allocated\n");
  return 0;
}

void
mutexclose(struct mutex *mutex)
{
  kfree((char*)mutex);
  printf("mutex has been closed\n");
}

