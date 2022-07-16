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

#ifdef LUNAPURPURA_PNG_SUPPORT
#include <stb_image_write.h>
#endif

#include "xpk.h"
#include "xpkdecoder.h"

/* ********** */

enum XPKToolAction {
	XPK_TOOL_ACTION_NONE,
	XPK_TOOL_ACTION_LIST,
	XPK_TOOL_ACTION_DECODE_INDIV,
	XPK_TOOL_ACTION_DECODE_TILED,
};

static void usage(void);
static bool xpk_list(const XPK *xpk);
static bool xpk_decode_indiv(const XPK *xpk);
static bool xpk_decode_tiled(const XPK *xpk);

static char *progname = NULL;
static enum XPKToolAction action = XPK_TOOL_ACTION_NONE;
static char *clu_path = NULL;
static char *out_path = NULL;
static char *xpk_path = NULL;
#ifdef LUNAPURPURA_PNG_SUPPORT
static bool want_png = false;
#endif
static uint8_t *rgba = NULL;

/* ********** */

static void
usage(void)
{
#ifdef LUNAPURPURA_PNG_SUPPORT
	const char *png_support = "[-p]";
#else
	const char *png_support = "";
#endif
	LPLog(
		LP_SUBSYSTEM_XPK,
		"usage: %s [options ...] [file]\n"
			"\t%s -l <file>                          List frames\n"
			"\t%s -x %s -c <clu> -o <output> <file>  Extract all frames\n"
			"\t%s -t %s -c <clu> -o <output> <file>  Extract single \"tiled mode\" frame\n"
			"\t%s -h                                 Show this help message",
		progname, progname, progname, png_support, progname, png_support,
		progname
	);
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

#ifdef LUNAPURPURA_PNG_SUPPORT
	const char *opts =  "c:hlo:ptx";
#else
	const char *opts =  "c:hlo:tx";
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
		case 'l':
			action = XPK_TOOL_ACTION_LIST;
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
			action = XPK_TOOL_ACTION_DECODE_TILED;
			break;
		case 'x':
			action = XPK_TOOL_ACTION_DECODE_INDIV;
			break;
		case '?': /* FALLTHROUGH */
		default:
			usage();
			return EXIT_FAILURE;
		}
	}
	argc -= optind;
	argv += optind;

	if (action == XPK_TOOL_ACTION_NONE) {
		usage();
		return EXIT_FAILURE;
	}

	if (argc < 1) {
		usage();
		return EXIT_FAILURE;
	}

	xpk_path = argv[0];
	LPStatus status;

	/* Obtain the XPK structure in any case. */
	FILE *xpk_fp = fopen(xpk_path, "rb");

	if (!xpk_fp) {
		LPWarn(LP_SUBSYSTEM_XPK, "can't read XPK: %s: %s", xpk_path, strerror(errno));
		return EXIT_FAILURE;
	}

	XPK *xpk = XPK_NewFromFile(xpk_fp, &status);

	if (!xpk) {
		fclose(xpk_fp);
		LPWarn(LP_SUBSYSTEM_XPK, "%s: %s", xpk_path, LPStatusString(status));
		return EXIT_FAILURE;
	}

	/* Obtain the CLU structure, if we need it, and attach it to the XPK. */
	if (action == XPK_TOOL_ACTION_DECODE_INDIV || action == XPK_TOOL_ACTION_DECODE_TILED) {
		if (!clu_path || !out_path) {
			usage();
			return EXIT_FAILURE;
		}

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
		XPK_AttachCLU(xpk, clu);
	}

	/* Now let's do the thing the user wants. */
	bool ok = false;

	switch (action) {
	case XPK_TOOL_ACTION_LIST:
		ok = xpk_list(xpk);
		break;
	case XPK_TOOL_ACTION_DECODE_INDIV:
		ok = xpk_decode_indiv(xpk);
		break;
	case XPK_TOOL_ACTION_DECODE_TILED:
		ok = xpk_decode_tiled(xpk);
		break;
	case XPK_TOOL_ACTION_NONE: /* NOTREACHED */
		;
		break;
	}

	XPKDecoder_FreeRGBA(rgba);
	XPK_Free(xpk);

	if (ok) {
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}
}


static bool
xpk_list(const XPK *xpk)
{
	printf("%d entry(ies):\n", xpk->n_entries);
	for (int i = 0; i < xpk->n_entries; i++) {
		printf("\t");
		XPKEntry_Pretty(XPK_EntryAtIndex(xpk, i));
	}
	return true;
}


static bool
xpk_decode_indiv(const XPK *xpk)
{
	LPStatus status;

	for (int i = 0; i < xpk->n_entries; i++) {
		XPKEntry *entry = XPK_EntryAtIndex(xpk, i);

		rgba = XPKEntry_Decode(entry, &status);

		if (status != LUNAPURPURA_OK) {
			LPWarn(LP_SUBSYSTEM_XPK, "couldn't decode: %s", LPStatusString(status));
			return false;
		}

		if (!(entry->width && entry->height)) {
			LPWarn(LP_SUBSYSTEM_XPK, "skipping entry %d with zero width and/or height", i);
			continue;
		}

		char real_out_path[512];

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

#ifdef LUNAPURPURA_PNG_SUPPORT
		if (!want_png)
#else
		if (true)
#endif
		{
			FILE *out_f = fopen(real_out_path, "wb"); /* XXX make sure this is ok */
			fwrite(rgba, 4, entry->width*entry->height, out_f);
			fclose(out_f);
		}

#ifdef LUNAPURPURA_PNG_SUPPORT
		if (want_png) {
			if ((status = XPKDecoder_RGBAToPNG(rgba, entry, real_out_path)) != LUNAPURPURA_OK) {
				LPWarn(LP_SUBSYSTEM_XPK, "couldn't write PNG!: %s", LPStatusString(status));
				return false;
			}
		}
#endif
	}

	return true;
}


static bool
xpk_decode_tiled(const XPK *xpk)
{
	if (!XPK_TiledModeOK(xpk)) {
		return false;
	}

	LPStatus status;
	rgba = XPK_DecodeTiledMode(xpk, &status);

	if (status != LUNAPURPURA_OK) {
		LPWarn(LP_SUBSYSTEM_XPK, "couldn't decode in tiled mode: %s", LPStatusString(status));
		return false;
	}

#ifdef LUNAPURPURA_PNG_SUPPORT
	if (!want_png)
#else
	if (true)
#endif
	{
		FILE *out_f = fopen(out_path, "wb"); /* XXX make sure this is ok */
		fwrite(rgba, 4, 640*480, out_f);
		fclose(out_f);
	}

#ifdef LUNAPURPURA_PNG_SUPPORT
	if (want_png) {
		int rv = stbi_write_png(out_path, 640, 480, 4, rgba, 640*4);
		if (!rv) {
			LPWarn(LP_SUBSYSTEM_XPK, "stbi_write_png() failed: %d", rv);
			return false;
		}
	}
#endif

	return true;
}
