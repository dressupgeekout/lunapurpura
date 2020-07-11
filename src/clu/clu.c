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
 * Creates a new CLU structure from the region of memory indicated by the
 * provided pointer.
 *
 * In most situations like this, we'd require the user to pass in a length,
 * but all CLUs have exactly the same length, so that's not necessary.
 *
 * This function creates a structure which is way more useful than the raw
 * region passed into it. Therefore, you ought to free the region when
 * successful.
 */
CLU *
CLU_NewFromData(uint8_t *region, LPStatus *status)
{
	CLU *clu = malloc(sizeof(CLU));

	if (!clu) {
		*status = LUNAPURPURA_ERROR;
		return NULL;
	}

	uint8_t *cluptr = NULL;
	uint8_t *regionptr = region;

	if (!ValidateMagic(region, CLU_MAGIC, CLU_MAGIC_LEN)) {
		*status = LUNAPURPURA_BADMAGIC;
		return NULL;
	}

	regionptr += CLU_MAGIC_LEN;

	for (int i = 0; i < CLU_NELEMENTS; i++) {
		memset(clu->array[i], 0, sizeof(clu->array[i]));
		cluptr = clu->array[i];

		regionptr += 2; /* skip marker */

		*cluptr = *regionptr; /* red */
		cluptr++;

		regionptr += 2;
		*cluptr = *regionptr; /* green */
		cluptr++;

		regionptr += 2;
		*cluptr = *regionptr; /* blue */

		regionptr += 2; /* go to next marker */
	}

	*status = LUNAPURPURA_OK;
	return clu;
}


/*
 * Creates a new CLU structure from the file at the given path. Reads the
 * entire file into memory.
 */
CLU *
CLU_NewFromFile(const char *path, LPStatus *status)
{
	FILE *f = fopen(path, "rb");

	if (!f) {
		*status = LUNAPURPURA_CANTOPENFILE;
		return NULL;
	}

	fseek(f, 0L, SEEK_END);
	long file_len = ftell(f);
	rewind(f);

	uint8_t *region = malloc(file_len);

	if (!region) {
		fclose(f);
		*status = LUNAPURPURA_ERROR;
		return NULL;
	}

	while (ftell(f) < file_len) {
		size_t n = fread(region, 1, BUFSIZ, f);
		region += n;
	}
	region -= file_len; /* "Rewind" the region */

	LPStatus clu_new_status;
	CLU *clu = CLU_NewFromData(region, &clu_new_status);

	if (!clu) {
		fclose(f);
		free(region);
		*status = clu_new_status;
		return NULL;
	}

	fclose(f);
	free(region);
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
