/*
 * prx/prxmember.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_PRXMEMBER_H
#define LUNAPURPURA_PRXMEMBER_H

#include "prx.h"

struct PrxMember {
	char filetype[PRXMEMBER_FILETYPE_LEN];
	unsigned char identifier;
	char name[PRXMEMBER_NAME_LEN];
	void *data;
};
typedef struct PrxMember PrxMember;

PrxMember *PrxMember_new(void);
void PrxMember_set_filetype(PrxMember *member, const char *filetype); 
void PrxMember_set_identifier(PrxMember *member, const unsigned char identifier);
void PrxMember_set_name(PrxMember *member, const char *name);
void PrxMember_delete(PrxMember *member);

#endif /* LUNAPURPURA_PRXMEMBER_H */
