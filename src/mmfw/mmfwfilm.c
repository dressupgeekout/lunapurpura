/*
 * This file is part of Luna Purpura.
 */

#include <stdio.h>
#include <stdlib.h>

#include "mmfw.h"
#include "mmfwfilm.h"

MMFWFilmEntry *
MMFWFilmEntry_New(MMFW *mmfw, int i)
{
	MMFWFilmEntry *film = malloc(sizeof(MMFWFilmEntry));

	if (!film) {
		return NULL;
	}

	uint32_t offset = mmfw->offsets[i];
	uint32_t length = mmfw->offsets[i+1] - offset;

	film->length = length;
	film->data = malloc(film->length);

	fseek(mmfw->fp, (long)offset, SEEK_SET);

	/* XXX one byte at a time :\ */
	uint8_t *datap = film->data;
	for (int i = 0; i < film->length; i++) {
		fread(datap, 1, 1, mmfw->fp);
		datap++;
	}

	return film;
}


void
MMFWFilmEntry_Free(MMFWFilmEntry *film)
{
	if (!film) return;
	if (film->data) free(film->data);
	free(film);
}
