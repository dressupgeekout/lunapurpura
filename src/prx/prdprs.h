/*
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_PRDPRS_H
#define LUNAPURPURA_PRDPRS_H

#include <stdio.h>

#include <lputil.h>

/* Forward declarations. */
struct PresageArchive;
typedef struct PresageArchive PresageArchive;


/*
 * Two file pointers are required in order to fully unarchive a PRD/PRS
 * pair.
 */
struct PRDPRS {
	FILE *header_fp;
	FILE *data_fp;
};
typedef struct PRDPRS PRDPRS;

PRDPRS *PRDPRS_New(const char *header_path, const char *data_path, LPStatus *status);
void PRDPRS_Close(PRDPRS *pair);

#endif /* LUNAPURPURA_PRDPRS_H */
