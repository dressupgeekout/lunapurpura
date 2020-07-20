/*
 * prx/prx.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_PRX_H
#define LUNAPURPURA_PRX_H

#include <stdio.h>

#include <lputil.h>

/* Forward declarations. */
struct PresageArchive;
typedef struct PresageArchive PresageArchive;


#define LP_SUBSYSTEM_PRX "prx"

#define PRX_MAGIC 1

struct PRX {
	FILE *fp;
};
typedef struct PRX PRX;

PRX *PRX_New(const char *path, LPStatus *status);
void PRX_Close(PRX *prx);

#endif /* LUNAPURPURA_PRX_H */
