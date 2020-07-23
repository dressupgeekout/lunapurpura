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
 * The FILE pointer is expected to have already been advanced to the
 * appropriate position.
 *
 * Don't forget to free the result with CLU_Free() you're done.
 */
CLU *
CLU_NewFromFile(FILE *fp, LPStatus *status)
{
	CLU *clu = malloc(sizeof(CLU));

	if (!clu) {
		*status = LUNAPURPURA_ERROR;
		return NULL;
	}

	uint8_t *cluptr = NULL;

	if (!ValidateMagicF(fp, CLU_MAGIC, CLU_MAGIC_LEN)) {
		*status = LUNAPURPURA_BADMAGIC;
		return NULL;
	}

	for (int i = 0; i < CLU_NELEMENTS; i++) {
		memset(clu->array[i], 0, sizeof(clu->array[i]));
		cluptr = clu->array[i];

		fseek(fp, 2L, SEEK_CUR); /* skip marker */
		ReadUint8(fp, 1, cluptr); /* R */

		cluptr++;
		fseek(fp, 1L, SEEK_CUR);
		ReadUint8(fp, 1, cluptr); /* G */

		cluptr++;
		fseek(fp, 1L, SEEK_CUR);
		ReadUint8(fp, 1, cluptr); /* B */

		fseek(fp, 1L, SEEK_CUR); /* go to next marker */
	}

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
