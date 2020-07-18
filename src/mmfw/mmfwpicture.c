/*
 * This file is part of Luna Purpura.
 */

#include <stdio.h>
#include <stdlib.h>

#include "mmfw.h"
#include "mmfwpicture.h"

MMFWPictureEntry *
MMFWPictureEntry_New(MMFW *mmfw, int i)
{
	MMFWPictureEntry *picture = malloc(sizeof(MMFWPictureEntry));

	if (!picture) {
		return NULL;
	}

	uint32_t offset = mmfw->offsets[i];
	uint32_t length = mmfw->offsets[i+1] - offset;

	picture->length = length;
	picture->data = malloc(picture->length);

	fseek(mmfw->fp, (long) offset, SEEK_SET);

	/* XXX one byte at a time :\ */
	uint8_t *datap = picture->data;
	for (int i = 0; i < picture->length; i++) {
		fread(datap, 1, 1, mmfw->fp);
		datap++;
	}

	return picture;
}


void
MMFWPictureEntry_Free(MMFWPictureEntry *picture)
{
	if (!picture) return;
	if (picture->data) free(picture->data);
	free(picture);
}
