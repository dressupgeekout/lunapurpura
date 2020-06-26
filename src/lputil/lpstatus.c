/*
 * lpstatus.c
 */

#include "lpstatus.h"

const char *
LPStatusString(const LPStatus status)
{
	switch (status) {
		case LUNAPURPURA_OK: return "OK"; break;
		case LUNAPURPURA_ERROR: return "Error"; break;
		case LUNAPURPURA_CANTOPENFILE: return "Can't open file"; break;
		case LUNAPURPURA_BADMAGIC: return "Bad magic"; break;
	}
}
