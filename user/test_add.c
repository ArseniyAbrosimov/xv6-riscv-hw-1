#include "kernel/types.h"
#include "user/user.h"


uint MAX = 21;
uint ERRREADLEN = 22;
uint ERRARGSLEN = 20;
uint ERRCHRLEN = 22;

uint SPACEASCII = 32;
uint MINUSASCII = 45;
uint MINIMUMNUM = 48;
uint MAXIMUMNUM = 57;


int
main(int argc, char *argv[])
{
  int i, cc;
  char c;
  char buf[MAX];

  int num2_pos = 0;
  
  int a_lth0 = 0, b_lth0 = 0;

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
    int is_minus = (c == MINUSASCII ? 1 : 0);
    int is_num = (c >= MINIMUMNUM && c <= MAXIMUMNUM ? 1 : 0);

    if (is_space) {
      if (i == 0 && num2_pos != 0) {
        write (2, "Wrong args (stdout)\n", ERRARGSLEN);
        exit(1);
      } else {
        num2_pos = i + 1;
      }
    } else if (is_minus) {
      if (i != 0 && i != num2_pos) {
        write (2, "Not a number (stdout)\n", ERRCHRLEN);
        exit(1);
      } else if (i == 0) {
        a_lth0 = 1;
      } else {
        b_lth0 = 1;
      }
    } else if (!is_num) {
      write (2, "Not a number (stdout)\n", ERRREADLEN);
      exit(1);     
    }
    buf[i++] = c;
  }

  buf[i] = '\0';
  if (i == num2_pos) {
    write (2, "Wrong args (stdout)\n", ERRARGSLEN);
    exit(1);
  }
  
  printf("|%s|\n", buf);

  int a = (a_lth0 ? -1 * atoi(buf + 1) : atoi(buf));
  int b = (b_lth0 ? -1 * atoi(buf + num2_pos + 1) : atoi(buf + num2_pos));
  
  int sum;
  sum = add(a, b);
  
  printf("%d\n", sum);
  exit(0);
}
