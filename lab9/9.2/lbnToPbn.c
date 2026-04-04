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
    return BAD_PBN;
}
