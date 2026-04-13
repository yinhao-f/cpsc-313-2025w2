#include "pgtable.h"
#include "paddr.h"

/* Physical address of this process's root (L1) page table */
paddr_t root_phys_pte_addr;

/* Initialize the address space. */
void addr_space_init()
{
    root_phys_pte_addr = phys_alloc_page();
}

int map_page(vaddr_t virt, paddr_t phys, unsigned char perms) {
  /* TODO: Implement map_page.
   *
   * 1. Reject bad inputs: virt must be below PADDR_OFFSET, and both
   *    virt and phys must be page-aligned.
   * 2. Compute the virtual address of the L1 PTE using PADDR_OFFSET
   *    and root_phys_pte_addr.
   * 3. If the L1 entry is not present, allocate a new page for the
   *    L2 table and install it.
   * 4. Compute the virtual address of the L2 PTE.
   * 5. If the L2 entry is already present, return -1 (must unmap first).
   * 6. Write the new L2 PTE with the provided physical address and
   *    permissions.
   *
   * Hint: PADDR_OFFSET + a_physical_address gives you a virtual address
   * that virt_read32/virt_write32 can use to access that physical location.
   */
  (void)virt; (void)phys; (void)perms;

  if (virt >= PADDR_OFFSET) return -1;
  if (OFST(virt) != 0) return -1;
  if (OFST(phys) != 0) return -1;

  paddr_t l1_pte_addr;
  paddr_t l2_page_addr;
  paddr_t l2_pte_addr;
  pte_t l1_pte;
  pte_t l2_pte;

  l1_pte_addr = root_phys_pte_addr + L1_INDEX(virt) * sizeof(pte_t);
  l1_pte = virt_read32(l1_pte_addr + PADDR_OFFSET);

  if (!PRES(l1_pte)) {
    l2_page_addr = phys_alloc_page();
    l1_pte = MAKE_PTE(l2_page_addr, perms);
    virt_write32(l1_pte_addr + PADDR_OFFSET, l1_pte);
  }

  l2_page_addr = PADDR(l1_pte);
  l2_pte_addr = l2_page_addr + L2_INDEX(virt) * sizeof(pte_t);
  l2_pte = virt_read32(l2_pte_addr + PADDR_OFFSET);
  if (PRES(l2_pte)) return -1;

  l2_pte = MAKE_PTE(phys, perms);
  virt_write32(l2_pte_addr + PADDR_OFFSET, l2_pte);

  return 0;
}

int unmap_page(vaddr_t virt) {
  /* TODO: Implement unmap_page.
   *
   * 1. Reject bad inputs: virt must be below PADDR_OFFSET and page-aligned.
   * 2. Walk the page table to find the L2 entry for virt.
   * 3. If the L1 or L2 entry is not present, return -1.
   * 4. Clear the L2 entry (write 0) to mark it not present.
   */
  // (void)virt;

  if (virt >= PADDR_OFFSET) return -1;
  if (OFST(virt) != 0) return -1;

  paddr_t l1_pte_addr;
  paddr_t l2_page_addr;
  paddr_t l2_pte_addr;
  pte_t l1_pte;
  pte_t l2_pte;

  l1_pte_addr = root_phys_pte_addr + L1_INDEX(virt) * sizeof(pte_t);
  l1_pte = virt_read32(l1_pte_addr + PADDR_OFFSET);

  if (!PRES(l1_pte)) return -1;

  l2_page_addr = PADDR(l1_pte);
  l2_pte_addr = l2_page_addr + L2_INDEX(virt) * sizeof(pte_t);
  l2_pte = virt_read32(l2_pte_addr + PADDR_OFFSET);
  if (!PRES(l2_pte)) return -1;

  virt_write32(l2_pte_addr + PADDR_OFFSET, 0);

  return 0;
}

paddr_t virt_to_phys(vaddr_t virt, unsigned char perms) {
  /*
   * Addresses at or above PADDR_OFFSET are  a direct window into  physical
   * memory. That is, we can access physical address x by accessing virtual
   * address PADDR_OFFSET + x.
   *
   * This is what lets map_page and unmap_page use virt_read32 / virt_write32
   * to manipulate page table entries stored in physical memory.
   */
  if (virt >= PADDR_OFFSET) {
    return virt - PADDR_OFFSET;
  }

  /* TODO: Implement the two-level page table walk.
   *
   * 1. Compute the virtual address of the L1 PTE for this virt.
   * 2. Read it. If not present, return PHYS_FAULT.
   * 3. Check permissions at L1.
   * 4. Extract the physical base of the L2 table from the L1 entry.
   * 5. Compute the virtual address of the L2 PTE, read it. If not
   *    present, return PHYS_FAULT.
   * 6. Check permissions (must be granted at both levels).
   * 7. Return the physical address: base from L2 entry | offset from virt.
   */
  (void)perms;

  if (virt >= PADDR_OFFSET) return -1;

  paddr_t l1_pte_addr;
  paddr_t l2_page_addr;
  paddr_t l2_pte_addr;
  pte_t l1_pte;
  pte_t l2_pte;

  l1_pte_addr = root_phys_pte_addr + L1_INDEX(virt) * sizeof(pte_t);
  l1_pte = virt_read32(l1_pte_addr + PADDR_OFFSET);

  if (!PRES(l1_pte)) return PHYS_FAULT;
  if (!(PERM(l1_pte) & perms)) return PHYS_FAULT;

  l2_page_addr = PADDR(l1_pte);
  l2_pte_addr = l2_page_addr + L2_INDEX(virt) * sizeof(pte_t);
  l2_pte = virt_read32(l2_pte_addr + PADDR_OFFSET);
  if (!PRES(l2_pte)) return PHYS_FAULT;
  if (!(PERM(l2_pte) & perms)) return PHYS_FAULT;

  return PADDR(l2_pte) | OFST(virt);
}
