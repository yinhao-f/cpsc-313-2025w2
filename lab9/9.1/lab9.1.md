# Directory Name Lookup 

<p>
Your goal in this problem is to implement the code that searches
a directory for a particular name, without using the <code>readdir</code>
function.
</p>
<p>
You are to write a function:
</p>

```c
CS313ino_t dirLookup(const char *filename, const char *component)
```

<p>
which takes a <code>filename</code>
that is the name of a file whose contents are a directory,
and a <code>component</code>
(i.e., part of path) and returns the inode number that corresponds to that
component name in the directory. If the component does not exist in the
directory, dirLookup should return BAD_DIR_ENTRY_INODE_NUMBER (which is 0).
</p>
<p>
Both the type definition for a CS313ino_t and the definition for BAD_DIR_ENTRY_INODE_NUMBER
are in the include file <code>dirLookup.h</code>.
</p>
<p>Important Note:  The normal (C) rules for stucture size and alignment don't apply to
directory entries.  No matter the sizes of the fields of a directory
entry, the size of each directory entry is rounded up to a multiple of 4 bytes.</p>
<p>
There are a couple of tricky cases to watch for.  Consider the following
sequence of operations.
</p>

```shell
% cat /usr/dict/words > foo
% cat /usr/dict/words > bar
% rm foo
% cat /usr/dict/words > name_longer_than_foo
% touch foo
```

At this point, three files exist in the directory: bar,
name_longer_than_foo, and foo.
However, it's possible that the contents of the directory at this point will
be:

```
INODE #     Name
0           foo
42          bar
128         name_longer_than_foo
1024        foo
```

<p>If <code>dirLookup</code> is called with a file containing this
directory and the component <code>foo</code>,
you might be tempted to return 0 as soon as you read that first entry, but
that would be incorrect; you must return 1024.
</p>
