/*
 * xpk/xpkdecoder.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_XPKDECODER_H
#define LUNAPURPURA_XPKDECODER_H

#include <stdbool.h>

#include <lputil.h>

#include <clu.h>

#include "xpk.h"

enum XPKDecoderInstruction {
	XPKINST_REPEAT = 0x0, /* Opcode 0 instead of argument 0 ... */
	XPKINST_RLENEXT = 0x00, /* ... BUT, opcode 0 AND argument 0 is a special case */
	XPKINST_REPEAT_END = 0x01,
	XPKINST_BIGREPEAT = 0x1,
	XPKINST_XSKIP = 0x2,
	XPKINST_BIGXSKIP = 0x3,
	XPKINST_RLE = 0x4,
	XPKINST_EOF = 0x41,
	XPKINST_BIGRLE = 0x5,
	XPKINST_DIRECT = 0x6,
	XPKINST_BIGDIRECT = 0x7,
	XPKINST_LINEREPEAT = 0x8,
	XPKINST_LINEREPEAT_END = 0x81,
	XPKINST_BIGLINEREPEAT = 0x9,
	XPKINST_SETXNL = 0xA,
	XPKINST_BIGSETXNL = 0xB,
	XPKINST_RLENL = 0xC,
	XPKINST_BIGRLENL = 0xD,
	XPKINST_DIRECTNL = 0xE,
	XPKINST_BIGDIRECTNL = 0xF,
};
typedef enum XPKDecoderInstruction XPKDecoderInstruction;

struct RepeatMarker {
	bool newline;
	unsigned int reps;
	long loc;
};

#define XPK_MAX_REPEAT_MARKERS 4

struct XPKDecoder {
	unsigned int n_reps;
	unsigned int direct_counter;
	uint16_t next_holder;
	size_t cur_x;
	size_t cur_y;
	struct RepeatMarker *repeat_markers[XPK_MAX_REPEAT_MARKERS];
	int repeat_marker_top;
};
typedef struct XPKDecoder XPKDecoder;

uint8_t *XPKDecoder_Decode(XPKDecoder *decoder, XPKEntry *entry, LPStatus *status);
void XPKDecoder_FreeRGBA(uint8_t *rgba);

#ifdef LUNAPURPURA_PNG_SUPPORT
LPStatus XPKDecoder_RGBAToPNG(const uint8_t *rgba, const XPKEntry *entry, const char *path);
#endif

#endif /* LUNAPURPURA_XPKDECODER_H */
