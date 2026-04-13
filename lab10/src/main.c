#include "pgtable.h"
#include "paddr_internal.h"
#include "paddr.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

static int tests_run    = 0;
static int tests_passed = 0;

static void check(const char *name, int pass)
{
    tests_run++;
    if (pass)
    {
        tests_passed++;
        printf("  PASS  %s\n", name);
    }
    else
    {
        printf("  FAIL  %s\n", name);
    }
}

int main(void)
{
    addr_space_init();

    uint32_t pa, pb;

    /* 1. Basic map and read-back */
    pa = phys_alloc_page();
    map_page(0x1000, pa, RD_PERM | WR_PERM);
    virt_write32(0x1000, 0xDEADBEEF);
    check("basicMapAndReadBack",
          virt_read32(0x1000) == 0xDEADBEEF);

    /* 2. Offset within a page is preserved */
    check("intraPageOffsetPreserved",
          virt_read32(0x1000 + 8) == 0);

    /* 3. Read from unmapped address returns 0 */
    check("unmappedReadReturnsZero",
          virt_read32(0x2000) == 0);

    /* 4. Two pages under the same L1 entry (same L1_INDEX, different L2_INDEX) */
    pa = phys_alloc_page();
    pb = phys_alloc_page();
    map_page(0x00400000, pa, RD_PERM | WR_PERM);
    map_page(0x00401000, pb, RD_PERM | WR_PERM);
    virt_write32(0x00400000, 0xAAAAAAAA);
    virt_write32(0x00401000, 0xBBBBBBBB);
    check("sameL1DifferentL2",
          virt_read32(0x00400000) == 0xAAAAAAAA &&
          virt_read32(0x00401000) == 0xBBBBBBBB);

    /* 5. Two pages under different L1 entries */
    pa = phys_alloc_page();
    pb = phys_alloc_page();
    map_page(0x00800000, pa, RD_PERM | WR_PERM);
    map_page(0x00C00000, pb, RD_PERM | WR_PERM);
    virt_write32(0x00800000, 0xCCCCCCCC);
    virt_write32(0x00C00000, 0xDDDDDDDD);
    check("differentL1Independent",
          virt_read32(0x00800000) == 0xCCCCCCCC &&
          virt_read32(0x00C00000) == 0xDDDDDDDD);

    /* 6. unmap makes the page inaccessible */
    virt_write32(0x00800000, 0x11111111);
    unmap_page(0x00800000);
    check("readAfterUnmapReturnsZero",
          virt_read32(0x00800000) == 0);

    /* 7. re-map to a different physical page after unmap succeeds */
    map_page(0x00800000, pb, RD_PERM | WR_PERM);
    virt_write32(0x00800000, 0x22222222);
    check("remapAfterUnmap",
          phys_read32(pb) == 0x22222222 &&
          phys_read32(pa) == 0x11111111);

    printf("\n%d / %d tests passed\n", tests_passed, tests_run);

    return tests_passed == tests_run ? 0 : 1;
}
