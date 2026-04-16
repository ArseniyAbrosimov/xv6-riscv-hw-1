#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
  uint64 nanosec = rtc();

  uint64 sec = nanosec / 1000000000ULL;
  uint32 rem_nanosec = (uint32)(nanosec % 1000000000ULL);

  uint64 days = sec / 86400;
  uint64 rem_sec = sec % 86400;

  int hour = rem_sec / 3600;
  int minute = (rem_sec % 3600) / 60;
  int second = rem_sec % 60;

  int year = 1970;
  for (;;) {
    int days_in_year = 365;
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
      days_in_year = 366;

    if (days >= days_in_year) {
      days -= days_in_year;
      year++;
    } else
      break;
  }

  
  int month = 1;
  int days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  for(;;) {
    int days_in_month = days_in_months[month-1];
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        days_in_month = 29;
    }
    if (days >= days_in_month) {
      days -= days_in_month;
      month++;
    } else
      break;
  }

  int day = days + 1;

  printf("%d-", year);
  if (month < 10)
    printf("0");
  printf("%d-", month);
  if (day < 10)
    printf("0");
  printf("%d", day);
  printf("T");
  if (hour < 10)
    printf("0");
  printf("%d:", hour);
  if (minute < 10)
    printf("0");
  printf("%d:", minute);
  if (second < 10)
    printf("0");
  printf("%d", second);
  printf(".");
  printf("%d\n", rem_nanosec);

  return 0;
}
