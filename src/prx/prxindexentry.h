/*
 * prx/prxindexentry.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_PRXINDEXENTRY_H
#define LUNAPURPURA_PRXINDEXENTRY_H

#include <stdint.h>
#include <stdio.h>

#include "prx.h"

struct PrxIndexEntry {
	char filetype[PRXMEMBER_FILETYPE_LEN];
	uint32_t id; // seems to be a globally unique identifier
	uint32_t size; // i would use size_t but is that guaranteed to be 32 bits wide?
	uint32_t __unknown1; // i think it's the index internal to this archive
	uint32_t __unknown2; // seems to be always zero
	uint32_t offset;
};
typedef struct PrxIndexEntry PrxIndexEntry;

PrxIndexEntry *PrxIndexEntry_new(const char *filetype, uint32_t id, uint32_t size, uint32_t offset);
PrxIndexEntry *PrxIndexEntry_read(FILE *fp);
size_t PrxIndexEntry_write(const PrxIndexEntry *entry, FILE *fp);
void PrxIndexEntry_delete(PrxIndexEntry *entry);

#endif /* LUNAPURPURA_PRXINDEXENTRY_H */
