# Implementing a Page Table Walk

In this question, your goal is to implement a page table walk to translate virtual
addresses into physical addresses. In the first part of the question, you need to
implement two functions:

```c
int map_page(vaddr_t virt, paddr_t phys, unsigned char perms);
int unmap_page(vaddr_t virt);
```

where <code>map_page</code> takes a virtual address <code>virt</code>, a physical address
<code>phys</code> and a set of permissions <code>perms</code>, and installs a mapping 
from <code>virt</code> to <code>phys</code>. Function <code>unmap_page</code> destroys the
mapping from <code>virt</code> to whatever physical page it's associated with.


The second part of this question asks you to implement

```c
paddr_t virt_to_phys(vaddr_t virt, unsigned char perms);
```

which performs the actual translation. Given a virtual address <code>virt</code> and the
requested operation (one of read/write/execute, encoded using the same constants as the
read/write/execute permissions in the page table entries), it returns the corresponding
physical address.

## Address Space Layout

You are implementing a 32-bit virtual address space over 4 MiB of physical memory
(represented by a flat buffer). Given 4 KiB pages, a single-level page table to handle
our entire address space would take up all of our physical memory before we ever got to
do anything interesting! We instead use a 2-level page table, described in more detail
in <code>pgtable.h</code>.

It's worth reading all of the provided files very carefully, as there are many helpful
macros, and they'll help you understand how the system fits together.


## Accessing Physical Memory

Something you'll notice quite quickly is that, while our page tables are expressed in
terms of physical addresses, the processors we know and love operate on virtual
addresses. Though we may be emulating the role of the MMU in address translation, we
don't have that same access to physical memory. In lieu of a true <em>dereference</em>
of our physical addresses, we've provided <code>virt_write32</code> and
<code>virt_read32</code> functions that write and read a 32-bit word. You should use
these functions in the implementation of your page table walk.

## Allocating Page Tables


Since page tables take up space, we need to set aside some memory to hold them.
<code>phys_alloc_page</code> is a bump allocator over physical memory provided precisely
for this: call it, and you'll receive the physical base address of a fresh page of
memory. While this is a very simple allocator, real kernels use similar techniques to
dynamically allocate memory before the virtual memory system is fully functional. Note
that this type of allocator doesn't support freeing of memory pages, and so, for
example, <code>unmap</code> cannot release a page back to the allocator, only
mark it not present.


## Edge Cases

<ul>
<li>If at any point an entry is not present when walking the page table,
<code>virt_to_phys</code> should return <code>PHYS_FAULT</code>.</li>
<li><code>map_page</code> should not allow mapping addresses above
<code>PADDR_OFFSET</code> (why?).</li>
</ul>
