/*
 * riff.c
 *
 * This file is part of Luna Purpura.
 */

#include <stdlib.h>

#include <lputil.h>

#include "riff.h"

RIFFChunk *
RIFFChunk_Create(uint32_t id, uint32_t size, uint8_t *data, LPStatus *status)
{
	RIFFChunk *chunk = malloc(sizeof(RIFFChunk));

	if (!chunk) {
		*status = LUNAPURPURA_ERROR;
		return NULL;
	}

	*status = LUNAPURPURA_OK;
	return chunk;
}


void
RIFFChunk_Free(RIFFChunk *chunk)
{
	free(chunk);
}
