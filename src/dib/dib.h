/*
 * dib/dib.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_DIB_H
#define LUNAPURPURA_DIB_H

#include <stdint.h>

#define DIB_TYPE "DIB"

typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;

struct BITMAPFILEHEADER {
	WORD bfType;
	DWORD bfSize;
	DWORD bfOffBits;
	WORD zeroes1; /* XXX real name? */
	WORD zeroes2; /* XXX real name? */
};

struct BITMAPINFOHEADER {
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	// ...
	// ...
};

/* */

struct DIB {
	struct BITMAPFILEHEADER BitMapFileHeader;
	struct BITMAPINFOHEADER BitMapInfoHeader;
	/* XXX CLUT here */
};
typedef struct DIB DIB;

void __dib_test__(void);

#endif /* LUNAPURPURA_DIB_H */
