/*
 * xpk main
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include <lpstatus.h>

#include "xpk.h"

static void usage(void);

int
main(int argc, char *argv[])
{
	if (argc <= 1) {
		usage();
		return EXIT_FAILURE;
	}

	const char *path = argv[1];

	LPStatus status;

	XPK *xpk = XPK_NewFromFile(path, &status);

	if (!xpk) {
		warnx("%s: %s", path, LPStatusString(status));
		return EXIT_FAILURE;
	}

	printf("%d entry(ies):\n", xpk->n_entries);

	for (int i = 0; i < xpk->n_entries; i++) {
		printf("\t");
		XPKEntry_Pretty(XPK_EntryAtIndex(xpk, i));
	}

	XPK_Free(xpk);
	return EXIT_SUCCESS;
}


static void
usage(void)
{
	warnx("usage: %s path", getprogname());
}
