#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"


uint32 seed = 1;
uint64 count = 0;

char
lcg(void)
{
  seed = (1103515245 * seed + 12345) & 0x7fffffff;
  return (char)(seed & 0xFF);
}

int
nullwrite(int user_src, uint64 src, int n)
{
  return n;
}

int
nullread(int user_dst, uint64 dst, int n)
{
  return 0;
}

int
zerowrite(int user_src, uint64 src, int n)
{
  return -1;
}

int
zeroread(int user_dst, uint64 dst, int n)
{
  uint i;
  char z = 0;
  for (i = 0; i < n; i++) {
    if(either_copyout(user_dst, dst, &z, 1) == -1)
      break;
  }
  return i;
}

int
urandomwrite(int user_src, uint64 src, int n)
{
  printf("test %d\n", n);
  if(n != sizeof(uint32))
    return -1;
  uint64 new_seed;
  if(either_copyin(&new_seed, user_src, src, sizeof(uint32)) == -1)
    return -1;
  seed = new_seed;
  return sizeof(uint32);
}

int
urandomread(int user_dst, uint64 dst, int n)
{
  for(int i = 0; i < n; i++){
      char r = lcg();
      if(either_copyout(user_dst, dst + i, &r, 1) == -1)
        return i;
  }
  return 0;
}
int
nullstatwrite(int user_src, uint64 src, int n)
{
  count += n;
  return n;
}

int
nullstatread(int user_dst, uint64 dst, int n)
{
  if(n != sizeof(uint64))
    return -1;

  if(either_copyout(user_dst, dst, &count, sizeof(uint64)) == -1)
    return -1;

  return sizeof(uint64);
}

int
driverwrite(int user_src, uint64 src, int n, short minor)
{
  if (minor == NULL_MINOR)
    return nullwrite(user_src, src, n);
  else if (minor == ZERO_MINOR)
    return zerowrite(user_src, src, n);
  else if (minor == URANDOM_MINOR)
    return urandomwrite(user_src, src, n);
  else if (minor == NULLSTAT_MINOR)
    return nullstatwrite(user_src, src, n);
  else {
    printf("utils driver %d does not exist", minor);
    return -1;
  }
}

int
driverread(int user_dst, uint64 dst, int n, short minor)
{
  if (minor == NULL_MINOR)
    return nullread(user_dst, dst, n);
  else if (minor == ZERO_MINOR)
    return zeroread(user_dst, dst, n);
  else if (minor == URANDOM_MINOR)
    return urandomread(user_dst, dst, n);
  else if (minor == NULLSTAT_MINOR)
    return nullstatread(user_dst, dst, n);
  else {
    printf("utils driver %d does not exist", minor);
    return -1;
  }
}

void
driverinit(void)
{
  seed = 1;
  count = 0;
  devsw[DRIVER].read = driverread;
  devsw[DRIVER].write = driverwrite;
}
