/*
 * xpk/xpk.c
 *
 * This file is part of Luna Purpura.
	*/

#include <stdio.h>
#include <stdlib.h>

#include <lputil.h>

#include <clu.h>

#include "xpk.h"
#include "xpkdecoder.h"

uint8_t XPK_MAGIC[XPK_MAGIC_LEN] = {165, 126, 112, 1};

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
	printf("XPKEntry offset=%d dimensions=%dx%d pos=%dx%d\n",
		entry->offset, entry->width, entry->height, entry->x, entry->y);
}


uint8_t *
XPKEntry_Decode(XPKEntry *entry, LPStatus *status)
{
	XPKDecoder decoder;
	return XPKDecoder_Decode(&decoder, entry, status);
}

/* ********** */

/**
 * The caller is responsible for ensuring the FILE* has advanced to the
 * correct place. The caller MUST NOT close the FILE* directly. Instead, use
 * XPK_Free() when you're done -- this will close the FILE* for you.
 */
XPK *
XPK_NewFromFile(FILE *f, LPStatus *status)
{
	if (!ValidateMagicF(f, XPK_MAGIC, XPK_MAGIC_LEN)) {
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
	xpk->file = f;

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
		entry->xpk = xpk;

		if (!entry) {
			*status = LUNAPURPURA_ERROR;
			fclose(f);
			return NULL;
		}

		xpk->entries[i] = entry;
	}

	/*
	 * The offsets which are listed in the XPK file are not absolute, they are
	 * relative to this position right here. But we'll go ahead and store the
	 * absolute offsets in the XPK struct we're trying to create. (Nothing
	 * wrong with that, right?)
	 *
	 * While generating this list, we'll enrich each of the XPKEntries we
	 * created earlier with this new info.
	 */
	uint32_t offsets_start_pos = ftell(f);

	/* XXX check for malloc error */
	xpk->offsets = calloc(xpk->n_entries, sizeof(uint32_t));

	for (int i = 0; i < xpk->n_entries; i++) {
		uint32_t offset;
		ReadUint32(f, 1, &offset);
		uint32_t absolute_offset = offsets_start_pos + offset;
		xpk->offsets[i] = absolute_offset;
		xpk->entries[i]->offset = absolute_offset;
	}

	*status = LUNAPURPURA_OK;
	return xpk;
}


void
XPK_Free(XPK *xpk)
{
	if (xpk->file) fclose(xpk->file);
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


/**
 * Attaching the CLU here transfers ownership to the XPK. Don't try to free the
 * XPK and then free the CLU directly, that's a double-free.
 */
void
XPK_AttachCLU(XPK *xpk, CLU *clu)
{
	xpk->clu = clu;
}


/*
 * Returns a 640x480 RGBA image. The whole thing is going to be in memory (1200
 * KB). The caller is expected to free the result, but we return NULL in case
 * of error.
 */
uint8_t *
XPK_DecodeTiledMode(const XPK *xpk, LPStatus *status)
{
	const unsigned int tile_count_x = 640/80; /* Number of tiles in the X direction */
	const unsigned int tile_count_y = 480/60; /* Number of tiles in the Y direction */
	size_t complete_cur_x = 0;
	size_t complete_cur_y = 0;
	LPStatus xpk_entry_decode_status;

	uint8_t *complete_rgba = calloc(640*480*4, 1);

	if (!complete_rgba) {
		*status = LUNAPURPURA_ERROR;
		return NULL;
	}

	XPKEntry *tile = NULL;

	/* (1) For each tile... */
	for (int tile_index = 0; tile_index < (tile_count_x*tile_count_y); tile_index++) {
		tile = XPK_EntryAtIndex(xpk, tile_index);
		uint8_t *tile_rgba = XPKEntry_Decode(tile, &xpk_entry_decode_status);

		if (xpk_entry_decode_status != LUNAPURPURA_OK) {
			free(complete_rgba);
			*status = xpk_entry_decode_status;
			return NULL;
		}

		/*
		 * The X and Y coordinates of the top-left corner of the current tile,
		 * in terms of the complete picture.
		 */
		complete_cur_x = tile->width * (tile_index % tile_count_x);
		complete_cur_y = tile->height * (tile_index / tile_count_y);

		/* (2) ...splat its contents into the correct spot in the grander image. */
		for (int y = 0; y < tile->height; y++) {
			for (int x = 0; x < tile->width; x++) {
				unsigned int this_color_pos = (y * tile->width * 4) + (x * 4);
				unsigned int this_pixel_pos = ((complete_cur_y+y) * 640 * 4) + ((complete_cur_x+x) * 4);
				complete_rgba[this_pixel_pos+0] = tile_rgba[this_color_pos+0];
				complete_rgba[this_pixel_pos+1] = tile_rgba[this_color_pos+1];
				complete_rgba[this_pixel_pos+2] = tile_rgba[this_color_pos+2];
				complete_rgba[this_pixel_pos+3] = tile_rgba[this_color_pos+3];
			}
		}
	}

	*status = LUNAPURPURA_OK;
	return complete_rgba;
}


/*
 * Indicates whether it is safe to attempt to decode with tiled mode. We say it
 * is OK if it has the correct number of entries, and every entry has the
 * expected dimensions.
 */
bool
XPK_TiledModeOK(const XPK *xpk)
{
	if (xpk->n_entries != XPK_TILED_TILE_COUNT) {
		LPWarn(LP_SUBSYSTEM_XPK, "doesn't meet criteria for tiled mode (%d entries != %d)", xpk->n_entries, XPK_TILED_TILE_COUNT);
		return false;
	} else {
		for (int i = 0; i < XPK_TILED_TILE_COUNT; i++) {
			int this_w = xpk->entries[i]->width;
			int this_h = xpk->entries[i]->height;
			if ((this_w != XPK_TILED_TILE_WIDTH) || (this_h != XPK_TILED_TILE_HEIGHT)) {
				LPWarn(
					LP_SUBSYSTEM_XPK,
					"doesn't meet criteria for tiled mode (entry #%d dimensions=%dx%d != %dx%d)",
					i, this_w, this_h, XPK_TILED_TILE_WIDTH, XPK_TILED_TILE_HEIGHT
				);
				return false;
			}
		}
	}

	return true;
}
