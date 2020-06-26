/*
 * prd/prdentry.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_PRDENTRY_H
#define LUNAPURPURA_PRDENTRY_H

#include <sys/types.h>

struct PRDEntry {
	int occurence_id;
	int id;
	int internal_id;
	int size;
	char ext[3];
	off_t offset;
	char *name;
};
typedef struct PRDEntry PRDEntry;

PRDEntry *PRDEntry_New();
void PRDEntry_Free(PRDEntry *entry);

#endif /* LUNAPURPURA_PRDENTRY_H */
