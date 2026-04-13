#include <string.h>

#include "paddr.h"
#include "paddr_internal.h"
#include "pgtable.h"

/* The buffer that plays the role of our physical memory. Page aligned. */
static uint8_t phys_mem[PHYS_MEM_SIZE] __attribute__ ((aligned (PAGE_SIZE)));

paddr_t phys_alloc_page(void) {
  static uint32_t cursor = 0;

  if (cursor + PAGE_SIZE > PHYS_MEM_SIZE)
  {
      return PHYS_FAULT;
  }

  paddr_t prev = cursor;
  memset(phys_mem + cursor, 0, PAGE_SIZE);
  cursor += PAGE_SIZE;
  return prev;
}

/*
 * The below functions are the only places that touch phys_mem directly, and
 * are only used  in the implementation of virtual read / write.
 */

/**
 * Reads the 32 bit word at physical address phys.
 * 
 * If the provided address is beyond the bounds of the underlying buffer, this
 * function segfaults.
 */
uint32_t phys_read32(paddr_t phys) {
  if (phys + sizeof(uint32_t) > PHYS_MEM_SIZE) {
    *(volatile char *)0 = 0;
  }
  return *(uint32_t *)(phys_mem + phys);
}

/**
 * Writes the provided 32 bit word at physical address phys.
 * 
 * If the provided address is beyond the bounds of the underlying buffer, this
 * function segfaults.
 */
void phys_write32(paddr_t phys, uint32_t val) {
  if (phys + sizeof(uint32_t) > PHYS_MEM_SIZE) {
    *(volatile char *)0 = 0;
  }
  *(uint32_t *)(phys_mem + phys) = val;
}
