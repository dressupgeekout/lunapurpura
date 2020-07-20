/*
 * This file is part of Luna Purpura.
 */

#include <stdio.h>
#include <stdlib.h>

#include <lputil.h>

#include "mmfw.h"
#include "mmfwsound.h"

const char *
MMFWSoundEntryFormat_String(const MMFWSoundEntryFormat fmt)
{
	switch (fmt) {
		case MMFW_SOUND_UNKNOWN: return "(unknown)"; break;
		case MMFW_SOUND_MONO_S16BE_22050: return "mono s16be @ 22050 Hz"; break;
		case MMFW_SOUND_MONO_S16BE_44100: return "mono s16be @ 44100 Hz"; break;
	}
}


MMFWSoundEntry *
MMFWSoundEntry_New(MMFW *mmfw, int i)
{
	MMFWSoundEntry *sound = malloc(sizeof(MMFWSoundEntry));

	if (!sound) {
		return NULL;
	}

	/*
	 * MMFW Sounds have 2 important bits of metadata.
	 */
	sound->unknown = MMFWSoundEntry_AForEntry(mmfw, i);
	sound->fmt = MMFWSoundEntry_FormatForEntry(mmfw, i);

	uint32_t offset = mmfw->offsets[i];
	uint32_t length = mmfw->offsets[i+1] - offset;

	sound->length = length;
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
	if (sound->unknown) free(sound->unknown);
	free(sound);
}


uint8_t *
MMFWSoundEntry_AForEntry(MMFW *mmfw, int i)
{
	fseek(mmfw->fp, mmfw->kind_metadata_start_pos, SEEK_SET);
	fseek(mmfw->fp, i*sizeof(uint32_t), SEEK_CUR);
	uint8_t *result = calloc(4, 1);
	ReadUint8(mmfw->fp, 1, &result[0]);
	ReadUint8(mmfw->fp, 1, &result[1]);
	ReadUint8(mmfw->fp, 1, &result[2]);
	ReadUint8(mmfw->fp, 1, &result[3]);
	return result;
}


MMFWSoundEntryFormat
MMFWSoundEntry_FormatForEntry(MMFW *mmfw, int i)
{
	fseek(mmfw->fp, mmfw->kind_metadata_start_pos, SEEK_SET);

	/* Skip over the "A" metadta. */
	fseek(mmfw->fp, mmfw->n_entries*sizeof(uint32_t), SEEK_CUR);

	/* I think we need this special flag. */
	//uint16_t flag;
	//ReadUint16(mmfw->fp, 1, &flag);
	fseek(mmfw->fp, 2L, SEEK_CUR);

	/* Navigate to the correct entry. */
	fseek(mmfw->fp, i*sizeof(unsigned char), SEEK_CUR);

	char result;
	ReadChar(mmfw->fp, 1, &result);

	switch (result) {
	case 0x15:
		return MMFW_SOUND_MONO_S16BE_22050;
		break;
	case 0x1D:
		return MMFW_SOUND_MONO_S16BE_44100;
		break;
	default:
		LPWarn(LP_SUBSYSTEM_MMFW, "unhandled MMFW Sound entry format %d for entry %d, setting to UNKNOWN", result, i);
		return MMFW_SOUND_UNKNOWN;
	}
}
