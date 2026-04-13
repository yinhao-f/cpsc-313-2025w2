#include "pgtable.h"
#include "paddr.h"
#include "paddr_internal.h"

uint32_t virt_read32(vaddr_t virt) {
  if (virt == 0) {
    *(volatile char *)0 = 0;
  }
  paddr_t paddr = virt_to_phys(virt, RD_PERM);
  if (paddr == PHYS_FAULT) {
    return 0;
  }
  return phys_read32(paddr);
}

void virt_write32(vaddr_t virt, uint32_t val) {
  if (virt == 0) {
    *(volatile char *)0 = 0;
  }
  paddr_t paddr = virt_to_phys(virt, WR_PERM);
  if (paddr == PHYS_FAULT) {
    return;
  }
  phys_write32(paddr, val);
}

void virt_exec32(vaddr_t virt, int *return_value) {
  if (virt == 0) {
    *(volatile char *)0 = 0;
  }
  paddr_t paddr = virt_to_phys(virt, EX_PERM);
  if (paddr == PHYS_FAULT) {
    *return_value = -1;
    return;
  }
  *return_value = 0;
}
