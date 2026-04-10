#ifndef PADDR_INTERNAL_H
#define PADDR_INTERNAL_H

#include <stdint.h>
#include "./paddr.h"

/*
 * Physical Memory
 *
 * We simulate physical memory using a simple flat buffer, 4MiB in size.
 * All "physical addresses" are byte offsets into this array.
 *
 * The buffer is mapped into the virtual address space at PADDR_OFFSET so
 * that our "kernel" can reach any physical byte through a known virtual
 * address.
 *
 */
uint32_t phys_read32(paddr_t phys);
void phys_write32(paddr_t phys, uint32_t val);

#endif /* PADDR_INTERNAL_H */

