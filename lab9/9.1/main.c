#include "dirLookup.h"

typedef struct _testcase {
	char *filename;
	char *to_lookup;
	CS313ino_t answer;
} testcase_t;

testcase_t tests[] = {
	{ "testSubset/image0", "Hamster", 63888 },
	{ "testSubset/image0", "Kite", 26487 },
	{ "testSubset/image1", "anything",BAD_DIR_ENTRY_INODE_NUMBER},
	{ "testSubset/image1", ".",55650},
	{ "testSubset/image1", "Piranha",38039},
	{ "testSubset/image1", "Yellow_perch",37403},
	{ "testSubset/image2", "split_seat_pack_speed_point_pool",8620},
	{ "testSubset/image2", "listen_forgotten_directly_sat_heard_she",61614},
	{ "testSubset/image2", "bad_component",BAD_DIR_ENTRY_INODE_NUMBER},
	{ "testSubset/image2", "..",57670},
	{ NULL, NULL, BAD_DIR_ENTRY_INODE_NUMBER }
};

int main(int argc, char *argv[]) {
	CS313ino_t retino;
	int t = 0, n_correct = 0;

	(void)argc;
	(void)argv;

	for (t = 0; tests[t].filename != NULL; t++) {
		retino = dirLookup(tests[t].filename, tests[t].to_lookup);
		if (retino != tests[t].answer) {
			fprintf(stderr, "File %s, component %s: returned %hu, expected %hu\n",
			    tests[t].filename, tests[t].to_lookup, retino, tests[t].answer);
		} else
			n_correct++;
	}
	printf("%d correct out of %d\n", n_correct, t);
	return 0;
}
