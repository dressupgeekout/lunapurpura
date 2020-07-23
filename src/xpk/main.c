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
	FILE *clu_fp = fopen(clu_path, "rb");

	if (!clu_fp) {
		LPWarn(LP_SUBSYSTEM_XPK, "can't read CLU: %s: %s", clu_path, strerror(errno));
		return EXIT_FAILURE;
	}

	CLU *clu = CLU_NewFromFile(clu_fp, &status);

	if (!clu) {
		LPWarn(LP_SUBSYSTEM_XPK, "can't read CLU: %s: %s", clu_path, LPStatusString(status));
		return EXIT_FAILURE;
	}

	fclose(clu_fp);

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

		for (int i = 0; i < xpk->n_entries; i++) {
			XPKEntry *entry = XPK_EntryAtIndex(xpk, i);

			rgba = XPKEntry_Decode(entry, &status);

			if (status != LUNAPURPURA_OK) {
				LPWarn(LP_SUBSYSTEM_XPK, "couldn't decode: %s", LPStatusString(status));
				goto fail;
			}

			if (!(entry->width && entry->height)) {
				LPWarn(LP_SUBSYSTEM_XPK, "skipping entry %d with zero width and/or height", i);
				continue;
			}

			char real_out_path[512];

			if (out_path) {
				if (xpk->n_entries > 1) {
					/*
					 * The number of XPK entries is a uint16, which means the largest
					 * possible frame number is 65535, which is 5 decimal digits long.
					 */
					snprintf(real_out_path, sizeof(real_out_path), "%05d-%s", i, out_path);
				} else {
					snprintf(real_out_path, sizeof(real_out_path), "%s", out_path);
				}
				LPWarn(LP_SUBSYSTEM_XPK, "decoding entry #%d to %s", i, real_out_path);
			}

#ifdef LUNAPURPURA_PNG_SUPPORT
			if (out_path && !want_png)
#else
			if (out_path)
#endif
			{
				FILE *out_f = fopen(real_out_path, "wb");
				fwrite(rgba, 4, entry->width*entry->height, out_f);
				fclose(out_f);
			}

#ifdef LUNAPURPURA_PNG_SUPPORT
			if (out_path && want_png) {
				if ((status = XPKDecoder_RGBAToPNG(rgba, entry, real_out_path)) != LUNAPURPURA_OK) {
					LPWarn(LP_SUBSYSTEM_XPK, "couldn't write PNG!: %s", LPStatusString(status));
					goto fail;
				}
			}
#endif
		}
	}

	XPKDecoder_FreeRGBA(rgba);
	XPK_Free(xpk);
	return EXIT_SUCCESS;

fail:
	XPKDecoder_FreeRGBA(rgba);
	XPK_Free(xpk);
	return EXIT_FAILURE;
}
