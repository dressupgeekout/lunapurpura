/*
 * presagearchive main.c
 *
 * This file is part of Luna Purpura.
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lputil.h>

#include "presagearchive.h"

/* ********** */

#define PARSE_OPTIONS_OK (-1)

/*
 * A RID is a 32-bit unsigned int, whose decimal expansion takes up to 10
 * characters, plus one for the separator. Therefore, 11.
 */
#define TOTAL_FILENAME_LEN (11 + PRESAGEARCHIVEMEMBER_NAME_LEN + PRESAGEARCHIVEMEMBER_FILETYPE_LEN)

enum PRXToolAction {
	PRX_TOOL_ACTION_NONE,
	PRX_TOOL_ACTION_LIST,
	PRX_TOOL_ACTION_EXTRACT,
};

static int parse_options(int argc, char *argv[]);
static void usage(void);

static char *progname = NULL;
static char *path1 = NULL;
static char *path2 = NULL;
static char *outname = NULL;
static bool verbose = false;
static enum PRXToolAction action = PRX_TOOL_ACTION_NONE;
static PresageArchiveKind kind;
static uint32_t which_asset = 0;
static char *which_filetype = NULL;
static bool want_asset_by_rid = false;
static bool want_all_assets = false;

/* ********** */

int
main(int argc, char *argv[])
{
#ifdef LUNAPURPURA_HAVE_GETPROGNAME
	progname = (char *)getprogname();
#else
	progname = argv[0];
#endif

	int rv;

	if (argc < 2) {
		usage();
		return EXIT_FAILURE;
	}

	if ((rv = parse_options(argc, argv)) != PARSE_OPTIONS_OK) {
		return rv;
	}

	if (verbose) {
		switch (kind) {
		case PRESAGE_ARCHIVE_KIND_PRDPRS:
			LPWarn(LP_SUBSYSTEM_PRX, "%s", "Assuming this is a PRD/PRS pair");
			break;
		case PRESAGE_ARCHIVE_KIND_PRX:
			LPWarn(LP_SUBSYSTEM_PRX, "%s", "Assuming this is a single PRX file");
			break;
		}
	}

	LPStatus status;
	PresageArchive *archive = PresageArchive_NewFromFiles(path1, path2, &status);

	if (!archive) {
		if (kind == PRESAGE_ARCHIVE_KIND_PRDPRS) {
			LPWarn(LP_SUBSYSTEM_PRX, "error: header=%s data=%s: %s", path1, path2, LPStatusString(status));
		} else {
			LPWarn(LP_SUBSYSTEM_PRX, "error: %s: %s", path1, LPStatusString(status));
		}
		if (status == LUNAPURPURA_CANTOPENFILE) {
			LPWarn(LP_SUBSYSTEM_PRX, "%s", strerror(errno));
		}
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
			max_i = want_all_assets ? archive->n_entries : which_asset;

			for (int i = min_i; i <= max_i; i++) {
				PresageArchiveMember *member = NULL;

				if (want_asset_by_rid) {
					member = PresageArchive_MemberWithResourceId(archive, which_filetype, which_asset);
				} else {
					member = archive->members[i-1];
				}

				if (!member) {
					const char *path = (kind == PRESAGE_ARCHIVE_KIND_PRX) ? path1 : path2;
					if (want_asset_by_rid) {
						LPWarn(LP_SUBSYSTEM_PRX, "%s: no such %s member with RID %ld", path, which_filetype, which_asset);
					} else {
						LPWarn(LP_SUBSYSTEM_PRX, "%s: no such member with ID %ld", path, which_asset);
					}
					continue;
				}

				FILE *fp = NULL;

				if (outname) {
					if (strcmp(outname, "-")) {
						fp = fopen(outname, "wb");
					} else {
						fp = stdout;
					}
				} else {
					char total_filename[TOTAL_FILENAME_LEN];
					snprintf(total_filename, TOTAL_FILENAME_LEN, "%u-%s.%s", member->rid, member->name, member->filetype);
					fp = fopen(total_filename, "wb");
				}

				if (!fp) {
					LPWarn(LP_SUBSYSTEM_PRX, "unable to extract member #%d (%s): %s", i, member->name, strerror(errno));
					goto fail;
				}

				(void)PresageArchive_DataForMember(archive, member);
				fwrite(member->data, member->size, 1, fp);
				PresageArchiveMember_FreeData(member);
				fclose(fp);

				if (verbose) {
					fprintf(stderr, "x\t%s.%s\n", member->name, member->filetype);
				}
			}
		}
		break;
	case PRX_TOOL_ACTION_LIST:
		{
			/* Merely print a list of all members. */
			printf(">> %d entries:\n", archive->n_entries);
			for (int i = 0; i < archive->n_entries; i++) {
				PresageArchiveMember *member = archive->members[i];
				printf("%-6d%-18s%-6s%-8d%-8d\n",
					member->internal_id-1, member->name, member->filetype, member->rid,
					member->size);
			}
		}
		break;
	case PRX_TOOL_ACTION_NONE: /* NOTREACHED */
		/* OK */
		break;
	}

	PresageArchive_Close(archive);
	return EXIT_SUCCESS;

fail:
	PresageArchive_Close(archive);
	return EXIT_FAILURE;
}


static int
parse_options(int argc, char *argv[])
{
	int ch;

	while ((ch = LPGetopt(argc, argv, "af:hn:o:r:tvx")) != -1) {
		switch (ch) {
		case 'a':
			want_all_assets = true;
			break;
		case 'f':
			which_filetype = optarg;
			break;
		case 'h':
			usage();
			return EXIT_SUCCESS;
			break;
		case 'n':
			which_asset = (uint32_t)atoi(optarg);
			break;
		case 'o':
			outname = optarg;
			break;
		case 'r':
			which_asset = (uint32_t)atoi(optarg);
			want_asset_by_rid = true;
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
	argc -= optind;
	argv += optind;

	if (argc < 1) {
		usage();
		return EXIT_FAILURE;
	}

	if (action == PRX_TOOL_ACTION_NONE) {
		usage();
		return EXIT_FAILURE;
	}

	if (action == PRX_TOOL_ACTION_EXTRACT) {
		if (!want_all_assets && !which_asset) {
			usage();
			return EXIT_FAILURE;
		}

		if (want_all_assets && (which_asset || outname)) {
			usage();
			return EXIT_FAILURE;
		}

		if (want_asset_by_rid && (!which_filetype || !which_asset)) {
			usage();
			return EXIT_FAILURE;
		}
	}

	if (argc >= 1) path1 = argv[0];
	if (argc >= 2) path2 = argv[1];

	if (path1 && path2) {
		kind = PRESAGE_ARCHIVE_KIND_PRDPRS;
	} else {
		kind = PRESAGE_ARCHIVE_KIND_PRX;
	}

	return PARSE_OPTIONS_OK;
}


static void
usage(void)
{
	LPWarn(
		LP_SUBSYSTEM_PRX,
		"usage: %s [options ...] [file1] [file2]\n"
			"\t%s -t <file>                                      List members\n"
			"\t%s -x [-v] [-o <path>] -n <ID> <file>             Extract a single member\n"
			"\t%s -x [-v] [-o <path>] -f <TYPE> -r <RID> <file>  Extract a single member by RID\n"
			"\t%s -x [-v] -a <file>                              Extract all members\n"
			"\t%s -h                                             Show this help message",
		progname, progname, progname, progname, progname, progname
	);
}
