/*
 * smf/smf.c
 *
 * This file is part of Luna Purpura.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <lpstatus.h>
#include <lputil.h>

#include "smf.h"

uint8_t SMF_MAGIC[SMF_MAGIC_LEN] = {173, 126, 121, 0};

/*
 * Creates a new SMF struct from the file at the given path.
 */
SMF *
SMF_NewFromFile(const char *path, LPStatus *status)
{
	FILE *f = fopen(path, "r");

	if (!f) {
		*status = LUNAPURPURA_CANTOPENFILE;
		return NULL;
	}

	if (!ValidateMagic(f, SMF_MAGIC, SMF_MAGIC_LEN)) {
		*status = LUNAPURPURA_BADMAGIC;
		return NULL;
	}

	SMF *smf = malloc(sizeof(SMF)); /* XXX error check */

	fclose(f);

	*status = LUNAPURPURA_OK;
	return smf;
}


void
SMF_Free(SMF *smf)
{
	free(smf);
}
