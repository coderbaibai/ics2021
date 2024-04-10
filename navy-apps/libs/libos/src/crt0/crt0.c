#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  int argc = *(int*)args;
  printf("argc1:%d\n",argc);
  char **argv = (char**)((int)args+sizeof(int));
  char **envp = (char**)((int)args+(argc+2)*sizeof(int));
  char *empty[] =  {NULL};
  printf("argc2:%d\n",argc);
  environ = empty;
  exit(main(argc, argv, envp));
  assert(0);
}
