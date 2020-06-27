/*
 * xpk main
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <lpstatus.h>

#include <clu.h>

#include "xpk.h"
#include "xpkdecoder.h"

/* ********** */

static void usage(void);

/* ********** */

static void
usage(void)
{
	warnx("usage: %s [-h] -c clu -o output file", getprogname());
}

/* ********** */

int
main(int argc, char *argv[])
{
	int ch;
	char *clu_path = NULL;
	char *out_path = NULL;

	while ((ch = getopt(argc, argv, "c:o:h")) != -1) {
		switch (ch) {
		case 'c':
			clu_path = optarg;
			break;
		case 'h':
			usage();
			return EXIT_SUCCESS;
			break;
		case 'o':
			out_path = optarg;
			break;
		case '?': /* FALLTHROUGH */
		default:
			usage();
			return EXIT_FAILURE;
		}
	}
	argc -= optind;
	argv += optind;

	if (!clu_path || argc < 1) {
		usage();
		return EXIT_FAILURE;
	}

	const char *xpk_path = argv[0];

	LPStatus status;

	/* Read the CLU. */
	CLU *clu = CLU_NewFromFile(clu_path, &status);

	if (!clu) {
		warnx("can't read CLU: %s: %s", clu_path, LPStatusString(status));
		return EXIT_FAILURE;
	}

	/* Read the XPK. */
	XPK *xpk = XPK_NewFromFile(xpk_path, &status);

	if (!xpk) {
		warnx("%s: %s", xpk_path, LPStatusString(status));
		return EXIT_FAILURE;
	}

	/* OK let's go! */
	XPK_AttachCLU(xpk, clu);

	printf("%d entry(ies):\n", xpk->n_entries);

	for (int i = 0; i < xpk->n_entries; i++) {
		printf("\t");
		XPKEntry_Pretty(XPK_EntryAtIndex(xpk, i));
	}

	/* We can only decode the 0th entry for now. */
	XPKEntry *entry = XPK_EntryAtIndex(xpk, 0);

	uint8_t *rgba = XPKEntry_Decode(entry, &status);

	if (status != LUNAPURPURA_OK) {
		warnx("couldn't decode: %s", LPStatusString(status));
		XPKDecoder_FreeRGBA(rgba);
		XPK_Free(xpk);
		return EXIT_FAILURE;
	}

	if (out_path) {
		FILE *out_f = fopen(out_path, "w");
		fwrite(rgba, 4, entry->width*entry->height, out_f);
		fclose(out_f);
	}

	XPKDecoder_FreeRGBA(rgba);
	XPK_Free(xpk);
	return EXIT_SUCCESS;
}
