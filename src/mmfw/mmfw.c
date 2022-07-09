/*
 * mmfw.c
 *
 * This file is part of Luna Purpura.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lputil.h>

#include "mmfw.h"

uint8_t MMFW_MAGIC[MMFW_MAGIC_LEN] = {'M', 'M', 'F', 'W'};

MMFW *
MMFW_NewFromFile(const char *path, LPStatus *status)
{
	MMFW *mmfw = malloc(sizeof(MMFW));

	if (!mmfw) {
		*status = LUNAPURPURA_ERROR;
		goto fail;
	}

	mmfw->fp = fopen(path, "rb");

	if (!mmfw->fp) {
		*status = LUNAPURPURA_CANTOPENFILE;
		goto fail;
	}

	if (!ValidateMagicF(mmfw->fp, MMFW_MAGIC, MMFW_MAGIC_LEN)) {
		*status = LUNAPURPURA_BADMAGIC;
		goto fail;
	}

	char mmfkind_s[MMFW_KIND_STR_LEN];
	fread(mmfkind_s, MMFW_KIND_STR_LEN, 1, mmfw->fp);

	/* XXX There's probably a better way of doing this. */
	if (!strncmp(mmfkind_s, MMFW_KIND_SOUNDS_STR, MMFW_KIND_STR_LEN)) {
		mmfw->kind = MMFW_KIND_SOUNDS;
	} else if (!strncmp(mmfkind_s, MMFW_KIND_PICTURES_STR, MMFW_KIND_STR_LEN)) {
		mmfw->kind = MMFW_KIND_PICTURES;
	} else if (!strncmp(mmfkind_s, MMFW_KIND_FILMS_STR, MMFW_KIND_STR_LEN)) {
		mmfw->kind = MMFW_KIND_FILMS;
	} else {
		*status = LUNAPURPURA_BADMAGIC;
		goto fail;
	}

	fseek(mmfw->fp, 4L, SEEK_CUR); /* [77, 77, 0, 2] */
	fseek(mmfw->fp, 14L, SEEK_CUR); /* not sure */
	ReadUint16(mmfw->fp, 1, &mmfw->n_entries);

	/*
	 * Obtain all the offsets for all the members.
	 *
	 * "+1" because there is one more offset stored in the file than there are
	 * members.  The last offset is always equal to the length of the file, which
	 * will be very helpful to know later on.
	 */
	mmfw->offsets = calloc(mmfw->n_entries+1, sizeof(uint32_t));

	for (uint16_t i = 0; i < (mmfw->n_entries+1); i++) {
		ReadUint32(mmfw->fp, 1, &(mmfw->offsets[i]));
	}

	fseek(mmfw->fp, 2L, SEEK_CUR); /* zeroes */

	/* Obtain the names of all the members. */
	char name[MMFW_ENTRY_NAME_LEN];

	mmfw->names = calloc(mmfw->n_entries, sizeof(MMFWEntryName));

	for (uint16_t i = 0; i < mmfw->n_entries; i++) {
		fread(name, 1, sizeof(name), mmfw->fp);
		snprintf(mmfw->names[i], MMFW_ENTRY_NAME_LEN, "%s", name);
	}

	/*
	 * What follows is metadata that is specific to the various kinds of MMFW
	 * archive. So let's keep track of our current file position.
	 */
	mmfw->kind_metadata_start_pos = ftell(mmfw->fp);

	*status = LUNAPURPURA_OK;
	return mmfw;

fail:
	if (mmfw) MMFW_Close(mmfw);
	return NULL;
}


void
MMFW_Close(MMFW *mmfw)
{
	if (!mmfw) return;
	fclose(mmfw->fp);
	if (mmfw->names) free(mmfw->names);
	free(mmfw);
}


/**
 * Be sure to free the result with MMFW_FreeEntry() when you're done.
 */
MMFWEntry *
MMFW_EntryAtIndex(MMFW *mmfw, int i)
{
	MMFWEntry *entry = malloc(sizeof(MMFWEntry));

	if (!entry) {
		return NULL;
	}

	entry->kind = mmfw->kind;

	switch (entry->kind) {
	case MMFW_KIND_SOUNDS:
		entry->entry.sound = MMFWSoundEntry_New(mmfw, i);
		break;
	case MMFW_KIND_PICTURES:
		entry->entry.picture = MMFWPictureEntry_New(mmfw, i);
		break;
	case MMFW_KIND_FILMS:
		entry->entry.film = MMFWFilmEntry_New(mmfw, i);
		break;
	default:
		; /* XXX NOTREACHED */
	}

	return entry;
}


/**
 * This also runs the "internal" free routine under the hood for you.
 */
void
MMFW_FreeEntry(MMFWEntry *entry)
{
	if (!entry) return;

	switch (entry->kind) {
	case MMFW_KIND_SOUNDS:
		MMFWSoundEntry_Free(entry->entry.sound);
		break;
	case MMFW_KIND_PICTURES:
		MMFWPictureEntry_Free(entry->entry.picture);
		break;
	case MMFW_KIND_FILMS:
		MMFWFilmEntry_Free(entry->entry.film);
		break;
	default:
		; /* XXX NOTREACHED */
	}

	free(entry);
}


uint32_t
MMFW_EntryLength(const MMFWEntry *entry)
{
	switch (entry->kind) {
	case MMFW_KIND_SOUNDS:
		return entry->entry.sound->length;
		break;
	case MMFW_KIND_PICTURES:
		return entry->entry.picture->length;
		break;
	case MMFW_KIND_FILMS:
		return entry->entry.film->length;
		break;
	default:
		return 0; /* XXX NOTREACHED */
	}
}


uint8_t *
MMFW_EntryData(const MMFWEntry *entry)
{
	switch (entry->kind) {
	case MMFW_KIND_SOUNDS:
		return entry->entry.sound->data;
		break;
	case MMFW_KIND_PICTURES:
		return entry->entry.picture->data;
		break;
	case MMFW_KIND_FILMS:
		return entry->entry.film->data;
		break;
	default:
		return NULL; /* XXX NOTREACHED */
	}
}

/**
 * Returns a string representation of the given MMFW's kind.
 */
const char *
MMFW_KindString(const MMFW *mmfw)
{
	switch (mmfw->kind) {
		case MMFW_KIND_NONE: return "(none)"; break;
		case MMFW_KIND_SOUNDS: return "Sounds"; break;
		case MMFW_KIND_PICTURES: return "Pictures"; break;
		case MMFW_KIND_FILMS: return "Films"; break;
	}
}
