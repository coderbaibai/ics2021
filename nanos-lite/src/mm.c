#include <memory.h>
#include <proc.h>
static void *pf = NULL;
extern Area heap;


void* new_page(size_t nr_page) {
  pf = (void*)((size_t)pf-(nr_page*PGSIZE));
  // printf("pf:%p\n",pf);
  return pf;
}

#ifdef HAS_VME
static void* pg_alloc(int n) {
  void * res = new_page(n/PGSIZE);
  memset(res,0,n);
  return res;
}
#endif

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
  printf("brk:%08x\n",brk);
  uint32_t former = current->max_brk;
  current->max_brk = brk;
  void* pg_addr;
  for(uint32_t i=(former&0xfffff000)+PGSIZE;i<current->max_brk;i+=PGSIZE){
    pg_addr = new_page(1);
    map(&current->as,(void*)i,pg_addr,0);
    // printf("mm_brk: map %08x to %08x\n",(char*)i,(char*)pg_addr);
  }
  return 0;
}

void init_mm() {
  // pf = (void *)ROUNDUP(heap.start, PGSIZE);
  pf = (void*)((size_t)heap.end);
  Log("free physical pages starting from %p", pf);

#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}
