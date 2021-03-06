/*
 * CLU reader program
 *
 * This file is part of Luna Purpura.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lputil.h>

#include "clu.h"

/* ********** */

static void usage(void);

static char *progname = NULL;

/* ********** */

static void
usage(void)
{
	LPLog("usage: %s file", progname);
}

/* ********** */

int
main(int argc, char *argv[])
{
#ifdef LUNAPURPURA_HAVE_GETPROGNAME
	progname = (char *)getprogname();
#else
	progname = argv[0];
#endif

	int ch;

	while ((ch = LPGetopt(argc, argv, "h")) != -1) {
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

	FILE *clu_fp = fopen(clu_path, "rb");

	if (!clu_fp) {
		LPWarn(LP_SUBSYSTEM_CLU, "%s: %s", clu_path, strerror(errno));
		return EXIT_FAILURE;
	}

	LPStatus status;
	CLU *clu = CLU_NewFromFile(clu_fp, &status);

	if (!clu) {
		fclose(clu_fp);
		CLU_Free(clu);
		LPWarn(LP_SUBSYSTEM_CLU, "%s: %s", clu_path, LPStatusString(status));
		return EXIT_FAILURE;
	}

	fclose(clu_fp);

	for (int i = 0; i < CLU_NELEMENTS; i++) {
		const uint8_t *color = CLU_ColorAtIndex(clu, i);
		printf("%d\t%d,%d,%d\n", i, color[0], color[1], color[2]);
	}

	CLU_Free(clu);
	return EXIT_SUCCESS;
}
