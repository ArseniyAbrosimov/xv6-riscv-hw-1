#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

#define Reg(reg) ((volatile uint32 *)(reg))

#define ReadReg(reg) (*(Reg(reg)))


static struct spinlock rtc_lock;

uint64
sys_rtc(void)
{
    uint32 low, high;

    acquire(&rtc_lock);

    low = ReadReg(RTC_LOW);
    high = ReadReg(RTC_HIGH);

    release(&rtc_lock);

    return ((uint64)high << 32) | low;
}

void
rtcinit(void)
{
  initlock(&rtc_lock, "rtc");
}

