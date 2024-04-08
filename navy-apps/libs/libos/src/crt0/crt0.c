#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  int argc = *(int*)args;
  printf("%08x\n",args);
  char **argv = (char**)((int)args+sizeof(int));
  char **envp = (char**)((int)args+(argc+2)*sizeof(int));
  printf("%08x\n",args);
  char *empty[] =  {NULL };
  environ = empty;
  printf("%s\n",argv[0]);
  exit(main(argc, argv, envp));
  assert(0);
}
