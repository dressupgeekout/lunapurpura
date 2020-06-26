/*
 * xpk/xpk.c
 *
 * This file is part of Luna Purpura.
	*/

#include <stdio.h>
#include <stdlib.h>

#include <lpstatus.h>
#include <lputil.h>

#include <clu.h>

#include "xpk.h"

uint8_t XPK_MAGIC[4] = {165, 126, 112, 1};

/* ********** */

XPKEntry *
XPKEntry_New(const size_t width, const size_t height, const int x, const int y)
{
	XPKEntry *entry = malloc(sizeof(XPKEntry)); /* XXX check for malloc error */
	if (!entry) {
		return NULL;
	}
	entry->width = width;
	entry->height = height;
	entry->x = x;
	entry->y = y;
	entry->data = NULL;
	return entry;
}


void
XPKEntry_Free(XPKEntry *entry)
{
	free(entry);
}


void
XPKEntry_Pretty(const XPKEntry *entry)
{
	printf("XPKEntry dimensions=%dx%d pos=%dx%d\n", entry->width, entry->height, entry->x, entry->y);
}

/* ********** */

XPK *
XPK_NewFromFile(const char *path, LPStatus *status)
{
	FILE *f = fopen(path, "r");

	if (!f) {
		*status = LUNAPURPURA_CANTOPENFILE;
		return NULL;
	}

	if (!ValidateMagic(f, XPK_MAGIC, XPK_MAGIC_LEN)) {
		*status = LUNAPURPURA_BADMAGIC;
		return NULL;
	}

	XPK *xpk = malloc(sizeof(XPK));

	/* XXX should explain bad malloc */
	if (!xpk) {
		free(xpk);
		*status = LUNAPURPURA_ERROR;
		return NULL;
	}

	xpk->clu = NULL;

	ReadUint8(f, 2, xpk->co2);
	ReadUint16(f, 1, &xpk->n_entries);
	xpk->a = 0; /* XXX dunno */
	fseek(f, 4L, SEEK_CUR);
	ReadUint32(f, 1, &xpk->next_section);

	xpk->entries = calloc(xpk->n_entries, sizeof(XPKEntry));

	for (int i = 0; i < xpk->n_entries; i++) {
		uint16_t width;
		uint16_t height;
		uint16_t x;
		uint16_t y;

		fseek(f, 8L, SEEK_CUR); /* Ignore first 4 numbers ("x1", "x2", "w1", "w2") */
		ReadUint16(f, 1, &x);
		ReadUint16(f, 1, &y);
		ReadUint16(f, 1, &width);
		ReadUint16(f, 1, &height);

		XPKEntry *entry = XPKEntry_New(width, height, x, y);

		if (!entry) {
			*status = LUNAPURPURA_ERROR;
			fclose(f);
			return NULL;
		}

		xpk->entries[i] = entry;
	}

	fclose(f);

	*status = LUNAPURPURA_OK;
	return xpk;
}


void
XPK_Free(XPK *xpk)
{
	for (int i = 0; i < xpk->n_entries; i++) {
		XPKEntry_Free(xpk->entries[i]);
	}
	CLU_Free(xpk->clu);
	free(xpk);

}


XPKEntry *
XPK_EntryAtIndex(const XPK *xpk, const int index)
{
	if (index > (xpk->n_entries-1)) {
		return NULL;
	}
	return xpk->entries[index];
}


/*
 * Attaching the CLU here transfers ownership to the XPK. Don't try to free the
 * XPK and then free the CLU directly, that's a double-free.
 */
void
XPK_AttachCLU(XPK *xpk, CLU *clu)
{
	xpk->clu = clu;
}
