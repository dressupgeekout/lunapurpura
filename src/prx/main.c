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

#include <lpstatus.h>

#include "prx.h"
#include "prxmember.h"

/* ********** */

#define PARSE_OPTIONS_OK (-1)

static int parse_options(int *argc, char **argv[]);
static void usage(void);

static char *path = NULL;
static bool verbose = false;
static bool want_extract = false;
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

	PRX *prx = PRX_NewFromFile(path, want_extract, &status);

	if (!prx) {
		warnx("error: %s: %s", path, LPStatusString(status));
		goto fail;
	}

	if (want_extract) {
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
	} else {
		/* Merely print a list of all members. */
		printf(">> %d entries:\n", prx->n_entries);
		for (int i = 0; i < prx->n_entries; i++) {
			PRXMember *member = prx->members[i];
			printf("%-6d%-18s%-6s%-8d%-8d\n",
				member->internal_id-1, member->name, member->filetype, member->identifier,
				member->size);
		}
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

	while ((ch = getopt(*argc, *argv, "ahn:vx")) != -1) {
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
		case 'v':
			verbose = true;
			break;
		case 'x':
			want_extract = true;
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

	if (want_extract && !want_all_assets && which_asset < 0) {
		usage();
		return EXIT_FAILURE;
	}

	if (want_extract && want_all_assets && which_asset >= 0) {
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
			"\t%s file\n"
			"\t%s -x [-n ID | -a] file\n"
			"\t%s -h",
		progname, progname, progname, progname
	);
}
