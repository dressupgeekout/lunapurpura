/*
 * prx/prxindexentry.c
 *
 * This file is part of Luna Purpura.
 */

#include <stdio.h>
#include <stdlib.h>

#include "prx.h"
#include "prxindexentry.h"

PrxIndexEntry *
PrxIndexEntry_new(const char *filetype, uint32_t id, uint32_t size, uint32_t offset)
{
	PrxIndexEntry *entry = calloc(1, sizeof(PrxIndexEntry));
	snprintf(entry->filetype, PRXMEMBER_FILETYPE_LEN, "%s", filetype);
	entry->id = id;
	entry->size = size;
	entry->offset = offset;
	return entry;
}


/*
 * The given file pointer is expected to have already been seeked to where a
 * PrxIndexEntry would be located.
 */
PrxIndexEntry *
PrxIndexEntry_read(FILE *fp)
{
	char filetype[PRXMEMBER_FILETYPE_LEN];
	uint32_t id;
	uint32_t size;
	uint32_t __unknown1;
	uint32_t __unknown2;
	uint32_t offset;

	fread(&filetype, PRXMEMBER_FILETYPE_LEN, 1, fp);
	fread(&id, sizeof(uint32_t), 1, fp);
	fread(&size, sizeof(uint32_t), 1, fp);
	fread(&__unknown1, sizeof(uint32_t), 1, fp);
	fread(&__unknown2, sizeof(uint32_t), 1, fp);
	fread(&offset, sizeof(uint32_t), 1, fp);

	PrxIndexEntry *entry = PrxIndexEntry_new(filetype, id, size, offset);
	entry->__unknown1 = __unknown1;
	entry->__unknown2 = __unknown2;
	return entry;
}


/*
 * Writes the given PrxIndexEntry to the file pointer, which is expected to
 * have already been seeked to an appropriate position.
 */
size_t
PrxIndexEntry_write(const PrxIndexEntry *entry, FILE *fp)
{
	return fwrite(entry, sizeof(PrxIndexEntry), 1, fp);
}


void
PrxIndexEntry_delete(PrxIndexEntry *entry)
{
	free(entry);
}
