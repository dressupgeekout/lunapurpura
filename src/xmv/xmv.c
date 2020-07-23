/*
 * xmv.c
 *
 * This file is part of Luna Purpura.
 */

#include <stdlib.h>
#include <stdio.h>

#include <lputil.h>

#include "xmv.h"

uint8_t XMV_MAGIC[XMV_MAGIC_LEN] = {186, 126, 109, 3};

void
XMVFrameSpec_Init(XMVFrameSpec *spec)
{
	spec->marker = 0;
	spec->xpkentry = 0;
	spec->a = 0;
	spec->b = 0;
	spec->c = 0;
	spec->d = 0;
	spec->e = 0;
	spec->f = 0;
}


/*
 * Be sure to free the result with XMV_Free() when you're done.
 *
 * The FILE* is assumed to have already been seeked to the appropriate
 * position.
 */
XMV *
XMV_NewFromFile(FILE *fp, LPStatus *status)
{
	XMV *xmv = calloc(1, sizeof(XMV));

	if (!xmv) {
		*status = LUNAPURPURA_ERROR;
		goto fail;
	}

	if (!ValidateMagicF(fp, XMV_MAGIC, XMV_MAGIC_LEN)) {
		*status = LUNAPURPURA_BADMAGIC;
		goto fail;
	}

	fseek(fp, (long)4 + 2 + 2 + 2 + 2, SEEK_CUR);

	ReadUint32(fp, 1, &(xmv->type));

	fseek(fp, 12L, SEEK_CUR);
	fseek(fp, 132L, SEEK_CUR); /* zeroes */

	ReadUint32(fp, 1, &(xmv->n_frames));

	xmv->frames = calloc(xmv->n_frames, sizeof(XMVFrameSpec));

	for (int i = 0; i < xmv->n_frames; i++) {
		ReadUint16(fp, 1, &(xmv->frames[i].marker));
		ReadUint16(fp, 1, &(xmv->frames[i].xpkentry));
		ReadUint16(fp, 1, &(xmv->frames[i].a));
		ReadUint16(fp, 1, &(xmv->frames[i].b));
		ReadUint16(fp, 1, &(xmv->frames[i].c));
		ReadUint16(fp, 1, &(xmv->frames[i].d));
		ReadUint16(fp, 1, &(xmv->frames[i].e));
		ReadUint16(fp, 1, &(xmv->frames[i].f));
	}

	*status = LUNAPURPURA_OK;
	return xmv;

fail:
	XMV_Free(xmv);
	return NULL;
}

void
XMV_Free(XMV *xmv)
{
	if (!xmv) return;
	if (xmv->frames) free(xmv->frames);
	free(xmv);
}
