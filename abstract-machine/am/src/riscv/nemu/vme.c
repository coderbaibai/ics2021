#include <am.h>
#include <nemu.h>
#include <klib.h>

static AddrSpace kas = {};
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
  NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0x80000000)

static inline void set_satp(void *pdir) {
  uintptr_t mode = 1ul << (__riscv_xlen - 1);
  asm volatile("csrw satp, %0" : : "r"(mode | ((uintptr_t)pdir >> 12)));
}

static inline uintptr_t get_satp() {
  uintptr_t satp;
  asm volatile("csrr %0, satp" : "=r"(satp));
  return satp << 12;
}

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  kas.ptr = pgalloc_f(PGSIZE);

  int i;
  for (i = 0; i < LENGTH(segments); i ++) {
    void *va = segments[i].start;
    for (; va < segments[i].end; va += PGSIZE) {
      map(&kas, va, va, 0);
    }
  }

  set_satp(kas.ptr);
  vme_enable = 1;

  return true;
}

void protect(AddrSpace *as) {
  PTE *updir = (PTE*)(pgalloc_usr(PGSIZE));
  as->ptr = updir;
  as->area = USER_SPACE;
  as->pgsize = PGSIZE;
  // map kernel space
  memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
  c->pdir = (vme_enable ? (void *)get_satp() : NULL);
}

void __am_switch(Context *c) {
  if (vme_enable && c->pdir != NULL) {
    set_satp(c->pdir);
  }
}

void map(AddrSpace *as, void *va, void *pa, int prot) {
  // printf("map %p to %p\n",va,pa);
  // 找到页目录项PTE所在地址，as的ptr默认已经初始化
  PTE* pte_outer = (PTE*)((uint32_t)as->ptr+4*(((uint32_t)va)>>22));
  // 找到页表
  PTE* pte_inner = NULL;
  // 如果页目录项为空，需要分配一个页来作为页表
  if(pte_outer->pte==0){
    pte_inner = (PTE*)(pgalloc_usr(PGSIZE));
    // 这片空间以4K为单位，高4K为基地址
    pte_outer->PPN_0 = (uint32_t)pte_inner>>12&0x3ff;
    pte_outer->PPN_1 = (uint32_t)pte_inner>>22&0x3ff;
    pte_outer->V = 1;
    // printf("outer:%08x\n",pte_outer->pte);
  }
  // 如果页目录项不为空，直接找到页表
  else pte_inner = (PTE*)(pte_outer->PPN_1<<22|pte_outer->PPN_0<<12|0);
  // 根据虚拟地址偏移，找到页表项
  pte_inner = (PTE*)((uint32_t)pte_inner+4*((uint32_t)va>>12&0x3ff));
  // 无论是否原有页表项，都进行覆盖(如果写了回收，则需要将覆盖的页帧回收)
  pte_inner->PPN_1 = (uint32_t)pa>>22&0x3ff;
  pte_inner->PPN_0 = (uint32_t)pa>>12&0x3ff;
  // 默认设置为有效
  pte_inner->V = 1;
  // printf("inner:%08x\n",pte_outer->pte);
}

Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
  Context* res = (Context*)(kstack.end-sizeof(Context));
  memset(res,0,sizeof(Context));
  res->mstatus = 0x1800;
  res->mepc = ((uintptr_t)entry)-sizeof(uintptr_t);
  res->SP = (uintptr_t)res;
  res->pdir = as->ptr;
  
  return res;
}
