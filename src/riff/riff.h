/*
 * riff.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_RIFF_H
#define LUNAPURPURA_RIFF_H

#include <lputil.h>

struct RIFFChunk {
	uint32_t id;
	uint32_t size;
	uint8_t *data;
};
typedef struct RIFFChunk RIFFChunk;

RIFFChunk *RIFFChunk_Create(uint32_t id, uint32_t size, uint8_t *data, LPStatus *status);
void RIFFChunk_Free(RIFFChunk *chunk);

/* ********** */

struct RIFF {
	RIFFChunk *chunks;
};
typedef struct RIFF RIFF;

#endif /* LUNAPURPURA_RIFF_H */
