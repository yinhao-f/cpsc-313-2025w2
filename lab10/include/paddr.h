#ifndef PADDR_H
#define PADDR_H

#include <stddef.h>
#include <stdint.h>

#define PHYS_FAULT (~(uint32_t)0)
#define PHYS_MEM_SIZE (4u << 20) /* 4 MiB                                 */
#define PADDR_OFFSET ((paddr_t)(0u - PHYS_MEM_SIZE)) /* 0xFFC00000       */

typedef uint32_t paddr_t;

/**
 * Public facing API of the bump allocator over physical memory. 
 * Hands out a fresh, zero-initialised page-aligned chunk from phys_mem.
 */
paddr_t phys_alloc_page(void);

#endif /* PADDR_H */
