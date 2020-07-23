/*
 * xmv.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_XMV_H
#define LUNAPURPURA_XMV_H

#include <stdio.h>

#include <lputil.h>

#define LP_SUBSYSTEM_XMV "xmv"

#define XMV_MAGIC_LEN 4
extern uint8_t XMV_MAGIC[XMV_MAGIC_LEN];

struct XMVFrameSpec {
	uint16_t marker;
	uint16_t xpkentry;
	uint16_t a;
	uint16_t b;
	uint16_t c;
	uint16_t d;
	uint16_t e;
	uint16_t f;
};
typedef struct XMVFrameSpec XMVFrameSpec;

void XMVFrameSpec_Init(XMVFrameSpec *spec);


struct XMV {
	uint32_t type;
	uint32_t n_frames;
	XMVFrameSpec *frames;
};
typedef struct XMV XMV;

XMV *XMV_NewFromFile(FILE *fp, LPStatus *status);
void XMV_Free(XMV *xmv);

#endif /* LUNAPURPURA_XMV_H */
