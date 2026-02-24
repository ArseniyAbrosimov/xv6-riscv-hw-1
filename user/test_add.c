#include "kernel/types.h"
#include "user/user.h"


uint MAX = 21;
uint ERRREADLEN = 22;
uint ERRARGSLEN = 20;
uint ERRCHRLEN = 22;

uint SPACEASCII = 32;
uint MINIMUMNUM = 48;
uint MAXIMUMNUM = 57;


int
main(int argc, char *argv[])
{
  int i, cc;
  char c;
  char buf[MAX];

  int num2_pos = 0;
  
  for(i=0; i+1 < MAX; ){
    cc = read(0, &c, 1);
    if(cc < 0) {
      write (2, "Read failure (stdout)\n", ERRREADLEN);
      exit(1);
    } else if (cc == 0) {
      break;
    }

    if(c == '\n')
      break;

    int is_space = (c == SPACEASCII ? 1 : 0);
    int is_num = (c >= MINIMUMNUM && c <= MAXIMUMNUM ? 1 : 0);

    if (is_space) {
      if (i == 0 && num2_pos != 0) {
        write (2, "Wrong args (stdout)\n", ERRARGSLEN);
        exit(1);
      } else {
        num2_pos = i + 1;
      }
    } else if (!is_num) {
      write (2, "Not a number (stdout)\n", ERRREADLEN);
      exit(1);     
    }
    buf[i++] = c;
  }

  buf[i] = '\0';

  if (i == num2_pos || num2_pos == 0) {
    write (2, "Wrong args (stdout)\n", ERRARGSLEN);
    exit(1);
  }
  
  printf("|%s|\n", buf);

  buf[num2_pos - 1] = '\0';

  int a = atoi(buf);
  int b = atoi(buf + num2_pos);
  
  int sum;
  sum = add(a, b);
  
  printf("%d\n", sum);
  exit(0);
}
