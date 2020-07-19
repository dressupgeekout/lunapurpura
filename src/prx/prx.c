/*
* prx/prx.c
*
* This file is part of Luna Purpura.
*/

#include <stdio.h>
#include <stdlib.h>

#include <lputil.h>

#include "presagearchive.h"
#include "prx.h"

PRX *
PRX_New(const char *path, LPStatus *status)
{
	PRX *prx = malloc(sizeof(PRX));

	if (!prx) {
		*status = LUNAPURPURA_ERROR;
		goto fail;
	}

	prx->fp = fopen(path, "rb");
	if (!prx->fp) {
		*status = LUNAPURPURA_CANTOPENFILE;
		goto fail;
	}

	*status = LUNAPURPURA_OK;
	return prx;

fail:
	PRX_Close(prx);
	return NULL;
}


void
PRX_Close(PRX *prx)
{
	if (!prx) return;
	if (prx->fp) fclose(prx->fp);
	free(prx);
}
