# Traversing Index Structures

<p>
Your goal in this assignment is to write code to traverse a file system
index structure so that you can translate logical block numbers into physical block numbers
(i.e., disk addresses).  Essentially, you are implementing code to do what
you did manually for the in-class exercise on "File Representation".
</p>

<p>
You are to write a function:
</p>

```c
uint64_t lbnToPbn(CS313inode_t *ip, uint64_t lbn, uint64_t block_size, block_t(*getBlock)(uint64_t block_no))
```

<p>
which takes a special CS313 inode (whose definition you will find in <code>lbnToPbn.h</code>),
a logical block number, the file system block size, and a function you should use to obtain
diskblocks, and returns the disk address where the specified logical block can be found.
</p>

<p>
<b>You will want to read lbnToPbn.h carefully</b> as it describes the particular
indexing structure that we are using for this exercise!
</p>

<p>
Your code must handle all the following conditions:</p>

<ul>
<li>If the lbn is beyond the end of the file, then lbnToPbn should return BAD_PBN.</li>
<li>If the lbn represents a hole in the file (a location at which no block has been
allocated) lbnToPbn should return HOLE_PBN, which is the physical block number
stored in the index structure for a hole in a file.</li>
</ul>

<p>
The last parameter, <code>getBlock</code> is a function pointer. Call it with
a physical disk address and it will return a block structure (see lbnToPbn.h)
that contains the contents of the disk block with the given physical block
number.
If the pbn is invalid, get_block returns an invalid block.
</p>
