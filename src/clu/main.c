/*
 * CLU reader program
 *
 * This file is part of Luna Purpura.
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <lpstatus.h>

#include "clu.h"

static void usage(void);

static void
usage(void)
{
	warnx("usage: %s file", getprogname());
}


int
main(int argc, char *argv[])
{
	int ch;

	while ((ch = getopt(argc, argv, "h")) != -1) {
		switch (ch) {
		case 'h':
			usage();
			return EXIT_SUCCESS;
			break;
		case '?': /* FALLTHROUGH */
		default:
			usage();
			return EXIT_FAILURE;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc < 1) {
		usage();
		return EXIT_FAILURE;
	}

	const char *clu_path = argv[0];
	LPStatus status;
	CLU *clu = CLU_NewFromFile(clu_path, &status);

	if (!clu) {
		CLU_Free(clu);
		warnx("%s: %s", clu_path, LPStatusString(status));
		return EXIT_FAILURE;
	}

	CLU_Pretty(clu);
	printf("========================================\n");

	for (int i = 0; i < 256; i++) {
		const uint8_t *color = CLU_ColorAtIndex(clu, i);
		printf("<%d,%d,%d>\n", color[0], color[1], color[2]);
	}

	CLU_Free(clu);
	return EXIT_SUCCESS;
}
