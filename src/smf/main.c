/*
 * smf
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <lpstatus.h>
#include <lputil.h>

#include "smf.h"

static void usage(void);


static void
usage(void)
{
	warnx("usage: %s [-h] file", getprogname());
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

	const char *path = argv[0];

	LPStatus status;

	SMF *smf = SMF_NewFromFile(path, &status);

	if (!smf) {
		warnx("couldn't open SMF file %s: %s", path, LPStatusString(status));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
