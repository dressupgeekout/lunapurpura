/*
 * prxreader.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_PRXREADER_H
#define LUNAPURPURA_PRXREADER_H

#include <stdio.h>

#include "prx.h"

struct PrxReader {
	char *path;
	FILE *fp;
};
typedef struct PrxReader PrxReader;

PrxReader *PrxReader_new(const char *path);
PrxFile *PrxReader_read(PrxReader *reader);
void PrxReader_delete(PrxReader *reader);

#endif /* LUNAPURPURA_PRXREADER_H */
