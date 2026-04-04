#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "lbnToPbn.h"

/**
 * A simple helper function. You do not have to use this.
 * @arg block: an indirect / double-indirect / triple-indirect block
 * @arg index: the index to access within the block
 * @returns a block pointer located at the "index"th location inside "block"
 */
static inline uint64_t get_pointer(block_t block, uint64_t index)
{
    return ((uint64_t *)&block.data)[index];
}

/**
 * @arg inode: an inode of a file
 * @arg lbn: a logical block number of the file
 * @arg block_size: the block size of the entire extended v6 file system
 * @arg getBlock: a function that takes a block number (block pointer) and returns the block itself
 * @returns the physical block number, translated from the logical block number using the inode
 *          BAD_PBN if lbn is a block beyond the end of a file
 *          HOLE_PBN if lbn is a "hole" in the file
 */
uint64_t lbnToPbn(CS313inode_t *inode, uint64_t lbn, uint64_t block_size, block_t(*getBlock)(uint64_t block_no))
{
    // some boundary checks
    if (lbn * block_size > inode->i_size) return BAD_PBN;

    uint64_t ptrs_per_block = block_size / sizeof(lbn);     // using lbn as the address size
    uint64_t max_blocks = NUM_DIRECT + ptrs_per_block * ptrs_per_block;
    uint64_t max_file_size = block_size * max_blocks;
    if (inode->i_size > max_file_size) return BAD_PBN;
    if (lbn >= max_blocks) return BAD_PBN;

    // look up pbn
    uint64_t pbn;
    block_t physical_block;
    if (lbn < NUM_DIRECT) {
        pbn = inode->i_direct[lbn];
        physical_block = getBlock(pbn);
        if (!physical_block.is_valid) return 0xA00BAD;
        return pbn;
    }

    lbn -= NUM_DIRECT;

    uint64_t indirect_index = lbn / ptrs_per_block;
    uint64_t pbn_index = lbn % ptrs_per_block;

    if (inode->i_2indirect == HOLE_PBN) return HOLE_PBN;
    block_t double_indir_block = getBlock(inode->i_2indirect);

    uint64_t indir_block_addr = get_pointer(double_indir_block, indirect_index);
    if (indir_block_addr == HOLE_PBN) return HOLE_PBN;
    block_t indir_block = getBlock(indir_block_addr);

    pbn = get_pointer(indir_block, pbn_index);

    return pbn;
}
