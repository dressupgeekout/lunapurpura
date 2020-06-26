/*
 * prd/prd.c
 *
 * This file is part of Luna Purpura.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <lputil.h>

#include "prdentry.h"
#include "prd.h"

uint8_t PRD_MAGIC[PRD_MAGIC_LEN] = {1, 0, 0, 0};

PRD *
PRD_NewFromFile(const char *path)
{
	FILE *f = fopen(path, "r");

	if (!f) {
		return NULL;
	}

	/* XXX should somehow warn about bad magic */
	if (!ValidateMagic(f, PRD_MAGIC, PRD_MAGIC_LEN)) {
		return NULL;
	}

	PRD *prd = malloc(sizeof(path)); /* XXX check for alloc error */

	fseek(f, 134L, SEEK_CUR);
	fseek(f, 2L, SEEK_CUR); /* "_unknown", usually the same as n_entires but not always */
	fread(&prd->n_entries, sizeof(uint16_t), 1, f);

	prd->entries = malloc(sizeof(PRDEntry)*prd->n_entries); /* XXX maybe calloc is better */
	for (int i = 0; i < prd->n_entries; i++) {
		prd->entries[i] = PRDEntry_New();
	}

	fclose(f);
	return prd;
}


void PRD_Free(PRD *prd)
{
	for (int i = 0; i < prd->n_entries; i++) {
		PRDEntry_Free(prd->entries[i]);
	}
	free(prd->entries);
	free(prd);
}
