/*
 * prd/main.c
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "prd.h"

static void usage(void);

static char *progname = NULL;

int
main(int argc, char *argv[])
{
#ifdef LUNAPURPURA_HAVE_GETPROGNAME
	progname = (char *)getprogname();
#else
	progname = argv[0];
#endif

	if (argc <= 1) {
		usage();
		return EXIT_FAILURE;
	}

	const char *path = argv[1];

	PRD *prd = PRD_NewFromFile(path);

	if (!prd) {
		warn("Can't open PRD file %s", path);
		return EXIT_FAILURE;
	}

	printf("n entries = %zu\n", prd->n_entries);
	PRD_Free(prd);

	return EXIT_SUCCESS;
}


static void
usage(void)
{
	warnx("usage: %s file", progname);
}
