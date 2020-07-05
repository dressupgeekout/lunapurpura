/*
 * prx/prx.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_PRX_H
#define LUNAPURPURA_PRX_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <lputil.h>

#include "prxmember.h"

#define LP_SUBSYSTEM_PRX "prx"

#define PRX_MAGIC 1

struct PRX {
	uint16_t n_entries;
	uint32_t data_offset_start;
	PRXMember **members;
};
typedef struct PRX PRX;

PRX *PRX_NewFromFile(const char *path, bool want_data, LPStatus *status);
void PRX_Free(PRX *prx);
uint32_t PRX_AbsoluteOffsetForMember(const PRX *prx, const int index);

#endif /* LUNAPURPURA_PRX_H */
