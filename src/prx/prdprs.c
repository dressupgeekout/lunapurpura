/*
 * This file is part of Luna Purpura.
 */

#include <stdio.h>
#include <stdlib.h>

#include <lputil.h>

#include "presagearchive.h"
#include "prdprs.h"

PRDPRS *
PRDPRS_New(const char *header_path, const char *data_path, LPStatus *status)
{
	PRDPRS *pair = malloc(sizeof(PRDPRS));

	if (!pair) {
		*status = LUNAPURPURA_ERROR;
		goto fail;
	}

	pair->header_fp = fopen(header_path, "rb");
	if (!pair->header_fp) {
		*status = LUNAPURPURA_CANTOPENFILE;
		goto fail;
	}

	pair->data_fp = fopen(data_path, "rb");
	if (!pair->data_fp) {
		*status = LUNAPURPURA_CANTOPENFILE;
		goto fail;
	}

	*status = LUNAPURPURA_OK;
	return pair;

fail:
	PRDPRS_Close(pair);
	return NULL;
}


void
PRDPRS_Close(PRDPRS *pair)
{
	if (!pair) return;
	if (pair->header_fp) fclose(pair->header_fp);
	if (pair->data_fp) fclose(pair->data_fp);
	free(pair);
}
