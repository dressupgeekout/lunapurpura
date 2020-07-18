/*
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_MMFWFILM_H
#define LUNAPURPURA_MMFWFILM_H

/* Forward declarations. */
struct MMFW;
typedef struct MMFW MMFW;

struct MMFWFilmEntry {
	uint32_t length;
	uint8_t *data;
};
typedef struct MMFWFilmEntry MMFWFilmEntry;

MMFWFilmEntry *MMFWFilmEntry_New(MMFW *mmfw, int i);
void MMFWFilmEntry_Free(MMFWFilmEntry *film);

#endif /* LUNAPURPURA_MMFWFILM_H */
