#ifndef PGTABLE_H
#define PGTABLE_H

#include <stddef.h>
#include <stdint.h>
#include "paddr.h"

/* 
 * Virtual Memory
 *
 * We support a 32 bit virtual address space, backed by physical memory as
 * described in paddr_internal.h
 *
 * We use a 2 level Page Table in order to represent 32 bits of virtual address
 * in a limited amount of space. Given we have 4KiB pages, a single level root
 * page table would take 4MiB to store, which is all of the available physical
 * memory.
 *
 * Since we have 32 bits to work with, and 4KiB pages, we give 10 bits of index
 * for the page tables. A Page Table Entry is 4 bytes in size, and so a Page
 * Table is exactly one page. The format of a virtual address is:
 *
 *  31       22 21        12 11          0
 * [ L1-index ] [ L2-index ] [  offset   ]
 *
 */
#define PAGE_SIZE (1u << 12)      /* 4 KiB                                */
#define PT_ENTRY_COUNT (1u << 10) /* 1024 pte_t entries in a table        */

#define OFST_MASK 0x00000FFFu
#define L2_INDEX_MASK 0x003FF000u
#define L1_INDEX_MASK 0xFFC00000u
#define L1_INDEX_SHIFT 22
#define L2_INDEX_SHIFT 12

#define OFST(v) ((v) & OFST_MASK)
#define L2_INDEX(v) (((v) & L2_INDEX_MASK) >> L2_INDEX_SHIFT)
#define L1_INDEX(v) (((v) & L1_INDEX_MASK) >> L1_INDEX_SHIFT)

/* Gets physical base address from a PTE. */
#define PADDR(v) ((v) & ~OFST_MASK)

/* 
 * Page table entry flags
 *
 * bit  0      present
 * bits 3:1    rwx permissions
 * bits 31:12  physical page base address
 *
 * Since the offset of a physical page address is irrelevant to our PTE (the
 * objects to which they point are page aligned), the lower 12 bits of a PTE
 * don't communicate meaningful information: they're free to steal for use as
 * flags.
 */
#define PRES_BIT 0x01u /* bit [0]     */

#define PERM_SHIFT 1
#define PERM_MASK 0x0Eu /* bits [3:1] */

#define RD_PERM 0x1u
#define WR_PERM 0x2u
#define EX_PERM 0x4u
#define FL_PERM (RD_PERM | WR_PERM | EX_PERM)

#define PRES(e) ((e) & PRES_BIT)
#define PERM(e) (((e) & PERM_MASK) >> PERM_SHIFT)

/* Build a PTE from a physical page address and permission bits. */
#define MAKE_PTE(paddr, perms)                                                 \
  (PADDR(paddr) | (((uint32_t)(perms) << PERM_SHIFT) & PERM_MASK) | PRES_BIT)

typedef uint32_t pte_t;
typedef uint32_t vaddr_t;

/* Virtual to Physical Address Translation */

/**
 * Initializes the virtual address space by setting up the root page table.
 */
void addr_space_init();

/**
 * Reads the 32 bit word at virtual address virt.
 *
 * If the provided address is NULL, the read will trigger a segfault.
 * Read may fail if the page is not mapped with the necessary permissions.
 */
uint32_t virt_read32(vaddr_t virt);

/**
 * Writes the provided 32 bit word to virtual address virt.
 *
 * If the provided address is NULL, the write will trigger a segfault.
 * Write may fail if the page is not mapped with the necessary permissions.
 */
void virt_write32(vaddr_t virt, uint32_t val);

/**
 * Tests execute permission on the virtual address virt.
 *
 * If the provided address is NULL, the exec will trigger a segfault.
 * Exec may fail if the page is not mapped with the necessary permissions.
 * Writes 0 through the return parameter on success, -1 on failure.
 */
void virt_exec32(vaddr_t virt, int *return_value);

/**
 * Creates a mapping from virtual address virt to physical address phys with
 * the specified permissions.
 *
 * If any level of the page table is not present, map_page will allocate a fresh
 * page of physical memory to hold the new page table and install it there. 
 *
 * The provided virtual address must not overlap with the PADDR_OFFSET range, as
 * this memory is already mapped.
 *
 * Provided addresses must be page-aligned.
 *
 * Returns `-1` on failure.
 */
int map_page(vaddr_t virt, paddr_t phys, unsigned char perms);

/**
 * Removes the mapping from virtual address virt to its corresponding physical
 * address.
 *
 * If the l1 entry in the page table is not present, there's nothing to unmap.
 * Unmap marks the corresponding l2 entry as not present.
 *
 * Provided address must be page-aligned.
 *
 * Returns `-1` on failure.
 */
int unmap_page(vaddr_t virt);

/**
 * Main driver of the virtual memory system, takes a virtual address and returns
 * the physical address to which it is mapped.
 *
 * This function must start at root_phys_pte_addr, and walk the page table.
 * If any of the pages on the page table walk lack the required permissions, a
 * fault is returned.
 *
 * Since the root of our page table is stored as a physical address, we need
 * some way of accessing this via a virtual address, that is, a known location.
 * 
 * Returns `PHYS_FAULT` on failure.
 */
paddr_t virt_to_phys(vaddr_t virt, unsigned char perms);

#endif /* PGTABLE_H */
