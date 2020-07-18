/*
 * mmfw.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_MMFW_H
#define LUNAPURPURA_MMFW_H

#include <stdint.h>
#include <stdio.h>

#include <lputil.h>

#include "mmfwsound.h"
#include "mmfwpicture.h"
#include "mmfwfilm.h"

#define LP_SUBSYSTEM_MMFW "mmfw"

#define MMFW_MAGIC_LEN 4
extern uint8_t MMFW_MAGIC[MMFW_MAGIC_LEN];

#define MMFW_KIND_STR_LEN 12
#define MMFW_KIND_SOUNDS_STR " Sounds"
#define MMFW_KIND_PICTURES_STR " Pictures"
#define MMFW_KIND_FILMS_STR " Films"

#define MMFW_ENTRY_NAME_LEN 32
typedef char MMFWEntryName[MMFW_ENTRY_NAME_LEN];

enum MMFWKind {
	MMFW_KIND_NONE,
	MMFW_KIND_SOUNDS,
	MMFW_KIND_PICTURES,
	MMFW_KIND_FILMS,
};
typedef enum MMFWKind MMFWKind;

union MMFWMetaEntry {
	MMFWSoundEntry *sound;
	MMFWPictureEntry *picture;
	MMFWFilmEntry *film;
};

struct MMFWEntry {
	MMFWKind kind;
	union MMFWMetaEntry entry;
};
typedef struct MMFWEntry MMFWEntry;

struct MMFW {
	FILE *fp;
	MMFWKind kind;
	uint16_t n_entries;
	uint32_t *offsets;
	MMFWEntryName *names;
};
typedef struct MMFW MMFW;

MMFW *MMFW_NewFromFile(const char *path, LPStatus *status);
void MMFW_Close(MMFW *mmfw);

MMFWEntry *MMFW_EntryAtIndex(MMFW *mmfw, int i);
void MMFW_FreeEntry(MMFWEntry *entry);
uint32_t MMFW_EntryLength(const MMFWEntry *entry);
uint8_t *MMFW_EntryData(const MMFWEntry *entry);

const char *MMFW_KindString(const MMFW *mmfw);

#endif /* LUNAPURPURA_MMFW_H */
