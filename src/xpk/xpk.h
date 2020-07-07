/*
 * xpk/xpk.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_XPK_H
#define LUNAPURPURA_XPK_H

#include <stdio.h>
#include <stdint.h>

#include <lputil.h>

#include <clu.h>

#define LP_SUBSYSTEM_XPK "xpk"

#define XPK_MAGIC_LEN 4

extern uint8_t XPK_MAGIC[XPK_MAGIC_LEN];

/* ********** */

/* Forward declaration. */
struct XPK;
typedef struct XPK XPK;

struct XPKEntry {
	XPK *xpk;
	uint32_t offset; /* Absolute start location within the XPK file */
	uint16_t width;
	uint16_t height;
	uint16_t x;
	uint16_t y;
	void *data;
};
typedef struct XPKEntry XPKEntry;

XPKEntry *XPKEntry_New(const size_t width, const size_t height, const int x, const int y);
void XPKEntry_Free(XPKEntry *entry);
void XPKEntry_Pretty(const XPKEntry *entry);
uint8_t *XPKEntry_Decode(XPKEntry *entry, LPStatus *status);

/* ********** */

struct XPK {
	FILE *file;
	uint8_t co2[2]; /* XXX part of the magic?? */
	uint16_t n_entries;
	int a; /* XXX unknown */
	uint32_t next_section;
	CLU *clu;
	uint32_t *offsets;
	XPKEntry **entries;
};

XPK *XPK_NewFromFile(const char *path, LPStatus *status);
void XPK_Free(XPK *xpk);
XPKEntry *XPK_EntryAtIndex(const XPK *xpk, const int index);
void XPK_AttachCLU(XPK *xpk, CLU *clu);

#endif /* LUNAPURPURA_XPK_H */
