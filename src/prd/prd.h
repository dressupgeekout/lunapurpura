/*
 * prd/prd.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_PRD_H
#define LUNAPURPURA_PRD_H

#include <stdint.h>

#include "prdentry.h"

#define PRD_MAGIC_LEN 4

extern uint8_t PRD_MAGIC[PRD_MAGIC_LEN];

struct PRD {
	size_t n_entries;
	PRDEntry **entries;
};
typedef struct PRD PRD;

PRD *PRD_NewFromFile(const char *path);
void PRD_Free(PRD *prd);

#endif /* LUNAPURPURA_PRD_H */
