#include <unistd.h>
#include <stdio.h>

int main() {
  int ret = write(1, "Hello World!\n", 13);
  printf("%d\n",ret);
  // int i = 2;
  // volatile int j = 0;
  // while (1) {
  //   j ++;
  //   if (j == 10000) {
  //     printf("Hello World from Navy-apps for the %dth time!\n", i ++);
  //     j = 0;
  //   }
  // }
  return 0;
}
