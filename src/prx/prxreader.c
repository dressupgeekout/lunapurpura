/*
 * prxreader.c
 *
 * This file is part of Luna Purpura.
 */

#include <stdio.h>
#include <stdlib.h>

#include "prx.h"
#include "prxreader.h"

PrxReader *
PrxReader_new(const char *path)
{
	PrxReader *reader = calloc(1, sizeof(PrxReader));
	reader->path = (char *)path;
	reader->fp = fopen(reader->path, "r");
	return reader;
}


PrxFile *
PrxReader_read(PrxReader *reader)
{
	return NULL;
}


void
PrxReader_delete(PrxReader *reader)
{
	fclose(reader->fp);
	free(reader);
}
