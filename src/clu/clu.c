/*
 * clu/clu.c
 *
 * This file is part of Luna Purpura.
 */

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lputil.h>

#include "clu.h"

uint8_t CLU_MAGIC[CLU_MAGIC_LEN] = {0, 0, 0, 0, 128, 0, 0, 255};


/*
 * Creates a new CLU structure from the file at the given path.
 */
CLU *
CLU_NewFromFile(const char *path, LPStatus *status)
{
	FILE *f = fopen(path, "rb");

	if (!f) {
		*status = LUNAPURPURA_CANTOPENFILE;
		return NULL;
	}

	if (!ValidateMagic(f, CLU_MAGIC, CLU_MAGIC_LEN)) {
		*status = LUNAPURPURA_BADMAGIC;
		return NULL;
	}

	CLU *clu = malloc(sizeof(CLU));
	if (!clu) {
		*status = LUNAPURPURA_ERROR;
		return NULL;
	}

	uint8_t *cluptr = NULL;

	for (int i = 0; i < CLU_NELEMENTS; i++) {
		memset(clu->array[i], 0, sizeof(clu->array[i]));
		cluptr = clu->array[i];

		fseek(f, 2L, SEEK_CUR); /* marker */

		fread(cluptr, 1, 1, f); /* red */
		fseek(f, 1L, SEEK_CUR);
		cluptr++;

		fread(cluptr, 1, 1, f); /* green */
		fseek(f, 1L, SEEK_CUR);
		cluptr++;

		fread(cluptr, 1, 1, f); /* blue */
		fseek(f, 1L, SEEK_CUR);
	}

	fclose(f);
	*status = LUNAPURPURA_OK;
	return clu;
}


void
CLU_Free(CLU *clu)
{
	free(clu);
}


/*
 * The caller doesn't need to free the result, it's statically allocated.
 */
const uint8_t *
CLU_ColorAtIndex(const CLU *clu, const int index)
{
	return clu->array[index];
}
