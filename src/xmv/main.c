/*
 * xmv main
 *
 * This file is part of Luna Purpura.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lputil.h>

#include "xmv.h"

/* ********** */

static void usage(void);

static char *progname = NULL;

/* ********** */

static void
usage(void)
{
	LPLog(LP_SUBSYSTEM_XMV, "usage: %s [-h] file", progname);
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

	XMV *xmv = NULL;

	const char *path = argv[0];
	FILE *xmv_fp = fopen(path, "rb");

	if (!xmv_fp) {
		LPWarn(LP_SUBSYSTEM_XMV, "couldn't open %s: %s", path, strerror(errno));
		goto fail;
	}

	LPStatus status;
	xmv = XMV_NewFromFile(xmv_fp, &status);

	if (status != LUNAPURPURA_OK) {
		LPWarn(LP_SUBSYSTEM_XMV, "couldn't read %s: %s", path, LPStatusString(status));
		goto fail;
	}

	fclose(xmv_fp);

	LPLog(LP_SUBSYSTEM_XMV, "%s: This is XMV type %d", path, xmv->type);
	LPLog(LP_SUBSYSTEM_XMV, "%d total frames", xmv->n_frames);

	for (int i = 0; i < xmv->n_frames; i++) {
		XMVFrameSpec frame = xmv->frames[i];
		printf(
			"frame=%02d\t%d\t%d,%d,%d,%d,%d,%d\n",
			i,
			frame.marker,
			frame.a, frame.b, frame.c, frame.d, frame.e, frame.f
		);
	}

	XMV_Free(xmv);
	return EXIT_SUCCESS;

fail:
	if (xmv_fp) fclose(xmv_fp);
	XMV_Free(xmv);
	return EXIT_FAILURE;
}
