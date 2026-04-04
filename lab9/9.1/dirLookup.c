#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>

#include "dirLookup.h"

#include <fcntl.h>
#include <unistd.h>
#define BYTES_BEFORE_NAME (sizeof(dir_entry_t) - (MAX_NAME_LEN + 1))

CS313ino_t dirLookup(const char *filename, const char *component) {
	int fd = open(filename, O_RDONLY);
	if (fd == -1) return BAD_DIR_ENTRY_INODE_NUMBER;	// File doesn't exist

	dir_entry_t entry;

	char buf[BYTES_BEFORE_NAME];

	while (read(fd, buf, BYTES_BEFORE_NAME) == BYTES_BEFORE_NAME) {
		entry.de_type = (uint8_t) buf[0];
		entry.de_namelen = (uint8_t) buf[1];

		entry.de_ino = (uint8_t) buf[4] + (((uint8_t) buf[5]) << 8);	// little-endian
		
		memset(entry.de_name, 0, sizeof(entry.de_name));
		int bytes_read = read(fd, entry.de_name, entry.de_namelen);

		if (bytes_read < entry.de_namelen) break;

		if ((strcmp(entry.de_name, component) == 0) && (entry.de_ino != 0)) return entry.de_ino;

		// skip trash bytes
		int trash_bytes = 4 - (entry.de_namelen + sizeof(entry.de_ino)) % 4;
		trash_bytes = trash_bytes % 4;
		lseek(fd, trash_bytes, SEEK_CUR);
	}

	return BAD_DIR_ENTRY_INODE_NUMBER;
}
