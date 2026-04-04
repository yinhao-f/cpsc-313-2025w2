#include "catch.hpp"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

extern "C"
{
#include "lbnToPbn.h"
}

// ***** We recommend that you test with some other block sizes! *****
#define N_RANDOM_TESTS 999
#define TEST_BLOCK_SIZE 1024
#define VERY_LARGE_NUMBER 0xFFFFFFFF

static block_t valid_block = {1};

block getBlock(uint64_t block_no) {
    printf("Filling block %x\n", block_no);
    block_t block = valid_block;
    const uint64_t ptrsPerBlock = TEST_BLOCK_SIZE / sizeof(uint64_t);
    uint64_t *pointers = (uint64_t *)&block.data;

    // if the input is good, fill the right index with the right data
    if (block_no == 0xFF) {
	for (uint64_t i = 0; i <= ptrsPerBlock; i++) {
	    pointers[i] = 0x100 + i;
	}
    } else if (0x100 <= block_no && block_no < 0x1000000 + ptrsPerBlock * ptrsPerBlock * ptrsPerBlock) {
    for (uint64_t i = 0; i <= ptrsPerBlock; i++) {
	    pointers[i] = 0x100 * block_no + i;
	}
    } else {
	// fill the block with bad data
	for (uint64_t i = 0; i < TEST_BLOCK_SIZE / 4; i++) {
	    pointers[i] = 0xBADBADBADBAD;
	}
    }
    return block;
}

TEST_CASE("test_direct_block_pointers", "[weight=1][part=test]")
{
    // block_size
    uint64_t block_size = TEST_BLOCK_SIZE;

    // inode
    CS313inode_t inode;

    // Small files test
    inode.i_size = NUM_DIRECT * block_size;

    for (uint64_t i = 0; i < NUM_DIRECT; i++) {
        inode.i_direct[i] = i + 0x100;
    }
    
    // test lbn -> pbn
    for (uint64_t lbn = 0; lbn < NUM_DIRECT; lbn++) {
        uint64_t expected_pbn = lbn + 0x100;
        uint64_t actual_pbn = lbnToPbn(&inode, lbn, block_size, getBlock);
        REQUIRE(actual_pbn == expected_pbn);
    }
}

TEST_CASE("test_double_indirect_block_pointer", "[weight=1][part=test]")
{
    // block_size
    uint64_t block_size = TEST_BLOCK_SIZE;
    const uint64_t ptrsPerBlock = block_size / sizeof(uint64_t);
    // inode
    CS313inode_t inode;

    // The file is the maximum size that requires a 2-level tree
    inode.i_size = block_size * (NUM_DIRECT + ptrsPerBlock * ptrsPerBlock);

    // All the direct pointers are invalid 
    for (int i = 0; i < NUM_DIRECT; i++) {
	inode.i_direct[i] = 0xBADBADBADBAD;
    }
    inode.i_2indirect = 0xFF;

    // Test each of the indirect block pointers
    // Notice how these numbers are similar to the ones in the
    // definition of getBlock
    for (int i = 0; i < ptrsPerBlock * ptrsPerBlock; i += ptrsPerBlock + 1) {
	uint64_t lbn = NUM_DIRECT + i;
	uint64_t l1 = (i / ptrsPerBlock) % ptrsPerBlock;
	uint64_t l2 = i % ptrsPerBlock;
	uint64_t expected_pbn = 0x10000 + l1 * 0x100 + l2;
	uint64_t actual_pbn = lbnToPbn(&inode, lbn, block_size, getBlock);
        REQUIRE(actual_pbn == expected_pbn);
    }
    REQUIRE(lbnToPbn(&inode, NUM_DIRECT + ptrsPerBlock * ptrsPerBlock, block_size, NULL) == BAD_PBN);
}

TEST_CASE("test_random_double_indirect_block_pointer", "[weight=1][part=test]")
{
    // block_size
    uint64_t block_size = TEST_BLOCK_SIZE;
    const uint64_t ptrsPerBlock = block_size / sizeof(uint64_t);
    // inode
    CS313inode_t inode;

    // The file is the maximum size that requires a 3-level tree
    inode.i_size = block_size * (NUM_DIRECT + ptrsPerBlock * ptrsPerBlock);

    // All the direct pointers are valid 
    for (int i = 0; i < NUM_DIRECT; i++) {
	inode.i_direct[i] = 0x100 + i;
    }
    inode.i_2indirect = 0xFF;

    // Test a bunch of random lbns
    for (int i = 0; i < N_RANDOM_TESTS; i++) {
	uint64_t lbn = rand() % (inode.i_size / block_size);
        uint64_t l1, l2;
	l1 = ((lbn - NUM_DIRECT) / ptrsPerBlock) % ptrsPerBlock;
	l2 = (lbn - NUM_DIRECT) % ptrsPerBlock;
        uint64_t expected_pbn = lbn < NUM_DIRECT ? 0x100 + lbn : 0x10000 + l1 * 0x100 + l2;
        uint64_t actual_pbn = lbnToPbn(&inode, lbn, block_size, getBlock);	
        REQUIRE(actual_pbn == expected_pbn);
    }
}

TEST_CASE("test_too_large", "[weight=1][part=test]"){
    // When the logical block number a caller requests is too large
    // (what is too large?), your lbnToPbn() should immediately return BAD_PBN
    // without needing to do any calculation.
    // **** Write your own test here. ****

    // block_size
    uint64_t block_size = TEST_BLOCK_SIZE;

    // inode
    CS313inode_t inode;
    inode.i_size = block_size;  // file size = 1 block
    
    // getBlock
    auto getBlock = [](uint64_t block_no) {
        return valid_block;
    };

    uint64_t lbn = VERY_LARGE_NUMBER;
    uint64_t expected_pbn = BAD_PBN;
    uint64_t actual_pbn = lbnToPbn(&inode, lbn, block_size, getBlock);

    REQUIRE(actual_pbn == expected_pbn);

    lbn = 2;    // just one block beyond EOF
    actual_pbn = lbnToPbn(&inode, lbn, block_size, getBlock);

    REQUIRE(actual_pbn == expected_pbn);
}
