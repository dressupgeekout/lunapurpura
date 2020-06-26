/*
* prx/prx.c
*
* This file is part of Luna Purpura.
*/

#include <stdio.h>
#include <stdlib.h>

#include "prx.h"

PrxHeader *
PrxHeader_new(size_t n_members)
{
	PrxHeader *header = calloc(1, sizeof(PrxHeader));
	header->magic = PRX_HEADER_MAGIC;
	header->__unknown1 = calloc(1, 137);
	header->n_members = n_members;
	header->__unknown2 = 0;
	header->__unknown3 = 0;
	return header;
}


PrxHeader *
PrxHeader_read(FILE *fp)
{
	return NULL; // XXX
}

size_t
PrxHeader_write(const PrxHeader *header, FILE *fp)
{
	return fwrite(header, sizeof(PrxHeader), 1, fp);
}


void
PrxHeader_delete(PrxHeader *header)
{
	free(header->__unknown1);
	free(header);
}

/* ********** */

PrxFile *
PrxFile_new(const PrxHeader *header)
{
	PrxFile *file = malloc(sizeof(PrxFile)); /* XXX check for error */
	return file;
}


PrxFile *
PrxFile_read(FILE *fp)
{
	return NULL; // XXX
}


void
PrxFile_add_member(PrxFile *prxfile, PrxMember *member)
{
	// XXX
}


void
PrxFile_delete(PrxFile *prxfile)
{
	PrxHeader_delete(prxfile->header);
	free(prxfile);
	// foreach member, PrxMember_delete(member)
}
