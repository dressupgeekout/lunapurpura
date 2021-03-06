/*
 * mmfwsound.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_MMFWSOUND_H
#define LUNAPURPURA_MMFWSOUND_H

/* Forward declarations. */
struct MMFW;
typedef struct MMFW MMFW;

enum MMFWSoundEntryFormat {
	MMFW_SOUND_UNKNOWN,
	MMFW_SOUND_MONO_S16BE_22050,
	MMFW_SOUND_MONO_S16BE_44100,
};
typedef enum MMFWSoundEntryFormat MMFWSoundEntryFormat;

const char *MMFWSoundEntryFormat_String(const MMFWSoundEntryFormat fmt);


struct MMFWSoundEntry {
	uint8_t *unknown;
	MMFWSoundEntryFormat fmt;
	uint32_t length;
	uint8_t *data;
};
typedef struct MMFWSoundEntry MMFWSoundEntry;

MMFWSoundEntry *MMFWSoundEntry_New(MMFW *mmfw, int i);
void MMFWSoundEntry_Free(MMFWSoundEntry *sound);
uint8_t *MMFWSoundEntry_AForEntry(MMFW *mmfw, int i);
MMFWSoundEntryFormat MMFWSoundEntry_FormatForEntry(MMFW *mmfw, int i);

#endif /* LUNAPURPURA_MMFWSOUND_H */
