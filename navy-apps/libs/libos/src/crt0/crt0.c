#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  int argc = *(int*)args;
  printf("call main:%d\n",argc);
  char **argv = (char**)((int)args+sizeof(int));
  char **envp = (char**)((int)args+(argc+2)*sizeof(int));
  char *empty[] =  {NULL};
  environ = empty;
  exit(main(argc, argv, envp));
  assert(0);
}
