/*
 * This file is part of Luna Purpura.
 */

#include <stdio.h>
#include <stdlib.h>

#include "mmfw.h"
#include "mmfwsound.h"

const char *
MMFWSoundEntryFormat_String(const MMFWSoundEntryFormat fmt)
{
	switch (fmt) {
		case MMFW_SOUND_UNKNOWN: return "(unknown)"; break;
		case MMFW_SOUND_MONO_S16BE_22050: return "mono s16be @ 22050 Hz"; break;
		case MMFW_SOUND_MONO_S16BE_44100: return "mono s16be @ 44100 hz"; break;
	}
}


MMFWSoundEntry *
MMFWSoundEntry_New(MMFW *mmfw, int i)
{
	MMFWSoundEntry *sound = malloc(sizeof(MMFWSoundEntry));

	if (!sound) {
		return NULL;
	}

	uint32_t offset = mmfw->offsets[i];
	uint32_t length = mmfw->offsets[i+1] - offset;

	sound->length = length;
	sound->fmt = MMFW_SOUND_UNKNOWN;
	sound->data = malloc(sound->length);

	fseek(mmfw->fp, (long)offset, SEEK_SET);

	/* XXX one byte at a time :\ */
	uint8_t *datap = sound->data;
	for (int i = 0; i < sound->length; i++) {
		fread(datap, 1, 1, mmfw->fp);
		datap++;
	}
	return sound;
}


void
MMFWSoundEntry_Free(MMFWSoundEntry *sound)
{
	if (!sound) return;
	if (sound->data) free(sound->data);
	free(sound);
}
