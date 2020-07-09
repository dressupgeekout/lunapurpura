/*
 * clu/clu.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_CLU_H
#define LUNAPURPURA_CLU_H

#include <stdint.h>

#include <lputil.h>

#define LP_SUBSYSTEM_CLU "clu"

#define CLU_TYPE "CLU"
#define CLU_NELEMENTS 256
#define CLU_MAGIC_LEN 8

extern uint8_t CLU_MAGIC[CLU_MAGIC_LEN];

struct CLU {
	uint8_t array[CLU_NELEMENTS][3];
};
typedef struct CLU CLU;

CLU *CLU_NewFromFile(const char *path, LPStatus *status);
void CLU_Free(CLU *clu);
const uint8_t *CLU_ColorAtIndex(const CLU *clu, const int index);

#endif /* LUNAPURPURA_CLU_H */
