/*
 * mmfwpicture.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_MMFWPICTURE_H
#define LUNAPURPURA_MMFWPICTURE_H

/* Forward declarations. */
struct MMFW;
typedef struct MMFW MMFW;

struct MMFWPictureEntry {
	uint32_t length;
	uint8_t *data;
};
typedef struct MMFWPictureEntry MMFWPictureEntry;

MMFWPictureEntry *MMFWPictureEntry_New(MMFW *mmfw, int i);
void MMFWPictureEntry_Free(MMFWPictureEntry *picture);

#endif /* LUNAPURPURA_MMFWPICTURE_H */
