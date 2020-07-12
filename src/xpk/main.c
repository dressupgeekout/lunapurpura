/*
 * xpk main
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lputil.h>

#include <clu.h>

#include "xpk.h"
#include "xpkdecoder.h"

/* ********** */

static void usage(void);

static char *progname = NULL;
static bool want_tiled_mode = false;

/* ********** */

static void
usage(void)
{
#ifdef LUNAPURPURA_PNG_SUPPORT
	const char *png_support = "[-p]";
#else
	const char *png_support = "";
#endif
	LPLog(LP_SUBSYSTEM_XPK, "usage: %s [-h] %s [-t] -c clu -o output file", progname, png_support);
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
	char *clu_path = NULL;
	char *out_path = NULL;

#ifdef LUNAPURPURA_PNG_SUPPORT
	bool want_png = false;
	const char *opts =  "c:o:pth";
#else
	const char *opts =  "c:o:th";
#endif

	while ((ch = LPGetopt(argc, argv, opts)) != -1) {
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
#ifdef LUNAPURPURA_PNG_SUPPORT
		case 'p':
			want_png = true;
			break;
#endif
		case 't':
			want_tiled_mode = true;
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
		LPWarn(LP_SUBSYSTEM_XPK, "can't read CLU: %s: %s", clu_path, LPStatusString(status));
		return EXIT_FAILURE;
	}

	/* Read the XPK. */
	XPK *xpk = XPK_NewFromFile(xpk_path, &status);

	if (!xpk) {
		LPWarn(LP_SUBSYSTEM_XPK, "%s: %s", xpk_path, LPStatusString(status));
		if (status == LUNAPURPURA_CANTOPENFILE) {
			LPWarn(LP_SUBSYSTEM_XPK, "%s", strerror(errno));
		}
		return EXIT_FAILURE;
	}

	/* OK let's go! */
	XPK_AttachCLU(xpk, clu);

	printf("%d entry(ies):\n", xpk->n_entries);

	uint8_t *rgba = NULL;

	if (want_tiled_mode) {
		rgba = XPK_DecodeTiledMode(xpk, &status);

		if (status != LUNAPURPURA_OK) {
			LPWarn(LP_SUBSYSTEM_XPK, "couldn't decode in tiled mode: %s", LPStatusString(status));
			goto fail;
		}

		FILE *out_f = fopen(out_path, "wb");
		fwrite(rgba, 4, 640*480, out_f);
		fclose(out_f);
	} else {
		for (int i = 0; i < xpk->n_entries; i++) {
			printf("\t");
			XPKEntry_Pretty(XPK_EntryAtIndex(xpk, i));
		}

		/* We can only decode the 0th entry for now. */
		XPKEntry *entry = XPK_EntryAtIndex(xpk, 0);

		rgba = XPKEntry_Decode(entry, &status);

		if (status != LUNAPURPURA_OK) {
			LPWarn(LP_SUBSYSTEM_XPK, "couldn't decode: %s", LPStatusString(status));
			goto fail;
		}

#ifdef LUNAPURPURA_PNG_SUPPORT
		if (out_path && !want_png)
#else
		if (out_path)
#endif
		{
			FILE *out_f = fopen(out_path, "wb");
			fwrite(rgba, 4, entry->width*entry->height, out_f);
			fclose(out_f);
		}

#ifdef LUNAPURPURA_PNG_SUPPORT
		if (out_path && want_png) {
			if ((status = XPKDecoder_RGBAToPNG(rgba, entry, out_path)) != LUNAPURPURA_OK) {
				LPWarn(LP_SUBSYSTEM_XPK, "couldn't write PNG!: %s", LPStatusString(status));
				goto fail;
			}
		}
#endif
	}

	XPKDecoder_FreeRGBA(rgba);
	XPK_Free(xpk);
	return EXIT_SUCCESS;

fail:
	XPKDecoder_FreeRGBA(rgba);
	XPK_Free(xpk);
	return EXIT_FAILURE;
}
