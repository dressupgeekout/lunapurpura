/*
 * This file is part of Luna Purpura.
 */

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <lputil.h>

#include "mmfw.h"

static void usage(void);

/* ********** */

enum MMFWToolMode {
	MMFW_TOOL_MODE_NONE,
	MMFW_TOOL_MODE_LIST,
	MMFW_TOOL_MODE_EXTRACT,
};

const char *progname = NULL;
enum MMFWToolMode mode = MMFW_TOOL_MODE_NONE;

/* ********** */

static void
usage(void)
{
	LPLog(
		LP_SUBSYSTEM_MMFW,
		"usage: %s [options ...] [file]\n"
		"\t%s -t <file>\n"
		"\t%s -x <file>",
		progname, progname, progname
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

	while ((ch = LPGetopt(argc, argv, "htx")) != -1) {
		switch (ch) {
		case 'h':
			usage();
			return EXIT_SUCCESS;
			break;
		case 't':
			mode = MMFW_TOOL_MODE_LIST;
			break;
		case 'x':
			mode = MMFW_TOOL_MODE_EXTRACT;
			break;
		case '?': /* FALLTHROUGH */
		default:
			usage();
			return EXIT_FAILURE;
		}
	}
	argc -= optind;
	argv += optind;

	if (mode == MMFW_TOOL_MODE_NONE) {
		usage();
		return EXIT_FAILURE;
	}

	if (argc < 1) {
		usage();
		return EXIT_FAILURE;
	}

	const char *path = argv[0];
	LPStatus status;
	MMFW *mmfw = MMFW_NewFromFile(path, &status);

	if (status != LUNAPURPURA_OK) {
		LPWarn(LP_SUBSYSTEM_MMFW, "couldn't read mmfw: %s", LPStatusString(status));
		if (status == LUNAPURPURA_CANTOPENFILE) {
			LPWarn(LP_SUBSYSTEM_MMFW, "%s", strerror(errno));
		}
		goto fail;
	}

	LPLog(
		LP_SUBSYSTEM_MMFW, ">> \"%s\" MMFW archive with %d members",
		MMFW_KindString(mmfw), mmfw->n_entries
	);

	for (uint16_t i = 0; i < mmfw->n_entries; i++) {
		switch (mode) {
		case MMFW_TOOL_MODE_LIST:
			{
				printf("%-32s\t%u\n", mmfw->names[i], mmfw->offsets[i]);
			}
			break;
		case MMFW_TOOL_MODE_EXTRACT:
			{
				MMFWEntry *entry = MMFW_EntryAtIndex(mmfw, i);
				FILE *outf = fopen(mmfw->names[i], "wb");
				fwrite(MMFW_EntryData(entry), 1, MMFW_EntryLength(entry), outf);
				fclose(outf);
				MMFW_FreeEntry(entry);
				fprintf(stderr, "x\t%s\n", mmfw->names[i]);
			}
			break;
		default:
			; /* NOTREACHED */
		}
	}

	MMFW_Close(mmfw);
	return EXIT_SUCCESS;

fail:
	MMFW_Close(mmfw);
	return EXIT_FAILURE;
}
