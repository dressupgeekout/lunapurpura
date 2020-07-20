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

static const char *progname = NULL;
static enum MMFWToolMode mode = MMFW_TOOL_MODE_NONE;
static bool want_extract_all = false;
static int which_member = -1;
static bool verbose = false;

/* ********** */

static void
usage(void)
{
	LPLog(
		LP_SUBSYSTEM_MMFW,
		"usage: %s [options ...] [file]\n"
		"\t%s -t <file>                List archive members\n"
		"\t%s -x [-v] -a <file>        Extract all members\n"
		"\t%s -x [-v] -n <id> <file>   Extract a single member",
		progname, progname, progname, progname
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

	while ((ch = LPGetopt(argc, argv, "ahn:tvx")) != -1) {
		switch (ch) {
		case 'a':
			want_extract_all = true;
			break;
		case 'h':
			usage();
			return EXIT_SUCCESS;
			break;
		case 'n':
			which_member = atoi(optarg);
			break;
		case 't':
			mode = MMFW_TOOL_MODE_LIST;
			break;
		case 'x':
			mode = MMFW_TOOL_MODE_EXTRACT;
			break;
		case 'v':
			verbose = true;
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

	if ((mode == MMFW_TOOL_MODE_EXTRACT) && want_extract_all && which_member >= 0) {
		usage();
		return EXIT_FAILURE;
	}

	if ((mode == MMFW_TOOL_MODE_EXTRACT) && !want_extract_all && (which_member < 0)) {
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
		LP_SUBSYSTEM_MMFW, "\"%s\" MMFW archive with %d members",
		MMFW_KindString(mmfw), mmfw->n_entries
	);

	uint16_t min_i = 0;
	uint16_t max_i = 0;

	switch (mode) {
	case MMFW_TOOL_MODE_LIST:
		min_i = 0;
		max_i = mmfw->n_entries;
		break;
	case MMFW_TOOL_MODE_EXTRACT:
		min_i = want_extract_all ? 0 : which_member;
		max_i = want_extract_all ? mmfw->n_entries : which_member+1;
		break;
	default:
		; /* NOTREACHED */
	}

	for (uint16_t i = min_i; i < max_i; i++) {
		switch (mode) {
		case MMFW_TOOL_MODE_LIST:
			{
				MMFWEntry *entry = MMFW_EntryAtIndex(mmfw, i);
				switch (entry->kind) {
				case MMFW_KIND_SOUNDS:
					{
						uint8_t *unknown = entry->entry.sound->unknown;
						printf(
							"%d\t%-32s\t%s\t(%d,%d,%d,%d)\t%u\n",
							i,
							mmfw->names[i],
							MMFWSoundEntryFormat_String(entry->entry.sound->fmt),
							unknown[0], unknown[1], unknown[2], unknown[3],
							mmfw->offsets[i]
						);
					}
					break;
				default:
					printf("%d\t%-32s\t%u\n", i, mmfw->names[i], mmfw->offsets[i]);
				}
			}
			break;
		case MMFW_TOOL_MODE_EXTRACT:
			{
				MMFWEntry *entry = MMFW_EntryAtIndex(mmfw, i);
				FILE *outf = fopen(mmfw->names[i], "wb");
				fwrite(MMFW_EntryData(entry), 1, MMFW_EntryLength(entry), outf);
				fclose(outf);
				MMFW_FreeEntry(entry);
				if (verbose) {
					fprintf(stderr, "x\t%s\n", mmfw->names[i]);
				}
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
