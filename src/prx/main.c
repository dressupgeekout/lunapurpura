/*
 * PRX main.c
 *
 * This file is part of Luna Purpura.
 */

#include <err.h>
#include <sys/errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <lputil.h>

#include "prx.h"
#include "prxmember.h"

/* ********** */

#define PARSE_OPTIONS_OK (-1)

enum PRXToolAction {
	PRX_TOOL_ACTION_NONE,
	PRX_TOOL_ACTION_LIST,
	PRX_TOOL_ACTION_EXTRACT,
};

static int parse_options(int *argc, char **argv[]);
static void usage(void);

static char *path = NULL;
static bool verbose = false;
static enum PRXToolAction action = PRX_TOOL_ACTION_NONE;
static int which_asset = -1;
static bool want_all_assets = false;

/* ********** */

int
main(int argc, char *argv[])
{
	int rv;

	if (argc < 2) {
		usage();
		return EXIT_FAILURE;
	}

	if ((rv = parse_options(&argc, &argv)) != PARSE_OPTIONS_OK) {
		return rv;
	}

	LPStatus status;

	PRX *prx = PRX_NewFromFile(path, (action == PRX_TOOL_ACTION_EXTRACT), &status);

	if (!prx) {
		warnx("error: %s: %s", path, LPStatusString(status));
		goto fail;
	}

	switch (action) {
	case PRX_TOOL_ACTION_EXTRACT:
		{
			/*
			 * Extract one OR all of the members in the archive, depending on the
			 * command-line options.
			 */
			int min_i;
			int max_i;
			min_i = want_all_assets ? 1 : which_asset;
			max_i = want_all_assets ? prx->n_entries-1 : which_asset;

			for (int i = min_i; i <= max_i; i++) {
				PRXMember *member = prx->members[i];
				const int TOTAL_FILENAME_LEN  = PRXMEMBER_NAME_LEN + PRXMEMBER_FILETYPE_LEN;
				char total_filename[TOTAL_FILENAME_LEN];
				snprintf(total_filename, TOTAL_FILENAME_LEN, "%s.%s", member->name, member->filetype);

				FILE *fp = fopen(total_filename, "w");
				if (!fp) {
					warnx("unable to extract member #%d (%s): %s", i, member->name, strerror(errno));
					goto fail;
				}
				fwrite(member->data, member->size, 1, fp);
				fclose(fp);

				if (verbose) {
					printf("x\t%s\n", total_filename);
				}
			}
		}
		break;
	case PRX_TOOL_ACTION_LIST:
		{
			/* Merely print a list of all members. */
			printf(">> %d entries:\n", prx->n_entries);
			for (int i = 0; i < prx->n_entries; i++) {
				PRXMember *member = prx->members[i];
				printf("%-6d%-18s%-6s%-8d%-8d\n",
					member->internal_id-1, member->name, member->filetype, member->identifier,
					member->size);
			}
		}
		break;
	case PRX_TOOL_ACTION_NONE: /* NOTREACHED */
		/* OK */
		break;
	}

	PRX_Free(prx);
	return EXIT_SUCCESS;

fail:
	PRX_Free(prx);
	return EXIT_FAILURE;
}


static int
parse_options(int *argc, char **argv[])
{
	int ch;

	while ((ch = getopt(*argc, *argv, "ahn:tvx")) != -1) {
		switch (ch) {
		case 'a':
			want_all_assets = true;
			break;
		case 'h':
			usage();
			return EXIT_SUCCESS;
			break;
		case 'n':
			which_asset = atoi(optarg);
			break;
		case 't':
			action = PRX_TOOL_ACTION_LIST;
			break;
		case 'v':
			verbose = true;
			break;
		case 'x':
			action = PRX_TOOL_ACTION_EXTRACT;
			break;
		case '?':
			/* FALLTHROUGH */
		default:
			usage();
			return EXIT_FAILURE;
		}
	}
	*argc -= optind;
	*argv += optind;

	if (*argc < 1) {
		usage();
		return EXIT_FAILURE;
	}

	if (action == PRX_TOOL_ACTION_NONE) {
		usage();
		return EXIT_FAILURE;
	}

	if (action == PRX_TOOL_ACTION_EXTRACT && !want_all_assets && which_asset < 0) {
		usage();
		return EXIT_FAILURE;
	}

	if (action == PRX_TOOL_ACTION_EXTRACT && want_all_assets && which_asset >= 0) {
		usage();
		return EXIT_FAILURE;
	}

	path = *argv[0];

	return PARSE_OPTIONS_OK;
}


static void
usage(void)
{
	const char *progname = getprogname();
	warnx(
		"usage: %s [options ...] [file]\n"
			"\t%s -t <file>               List members\n"
			"\t%s -x [-v] -n <ID> <file>  Extract a single member\n"
			"\t%s -x [-v] -a <file>       Extract all members\n"
			"\t%s -h                      Show this help message",
		progname, progname, progname, progname, progname
	);
}
