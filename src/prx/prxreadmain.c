/* 
 * prxreadmain.c
 *
 * This file is part of Luna Purpura.
 */
 
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "prx.h"
#include "prxreader.h"

#define PARSE_OPTIONS_OK (-1)

static int parse_options(int argc, char *argv[]);
static void usage(void);

static char *path = NULL;
static bool verbose = false;

int
main(int argc, char *argv[])
{
	int rv;

	if (argc < 2) {
		usage();
		return EXIT_FAILURE;
	}

	if ((rv = parse_options(argc, argv)) != PARSE_OPTIONS_OK) {
		return rv;
	}

	path = argv[1];

	PrxReader *reader = PrxReader_new(path);
	PrxFile *file = NULL;
	file = PrxReader_read(reader);
	PrxFile_delete(file);
	PrxReader_delete(reader);
	return EXIT_SUCCESS;
}

static int
parse_options(int argc, char *argv[])
{
	int ch;

	while ((ch = getopt(argc, argv, "hv")) != -1) {
		switch (ch) {
		case 'h':
			usage();
			return EXIT_SUCCESS;
			break;
		case 'v':
			verbose = true;
			break;
		case '?':
			/* FALLTHROUGH */
		default:
			usage();
			return EXIT_FAILURE;
		}
	}

	return PARSE_OPTIONS_OK;
}

static void
usage(void)
{
	warnx("usage: %s [-hv] file", getprogname());
}
