/*
 * prx/prxmember.c
 *
 * This file is part of Luna Purpura.
 */

#include <stdio.h>
#include <stdlib.h>

#include "prx.h"
#include "prxmember.h"

PrxMember *
PrxMember_new(void)
{
	PrxMember *member = calloc(1, sizeof(PrxMember));
	return member;
}


void
PrxMember_set_filetype(PrxMember *member, const char *filetype)
{
	snprintf(member->filetype, PRXMEMBER_FILETYPE_LEN, "%s", filetype);
}


void
PrxMember_set_identifier(PrxMember *member, const unsigned char identifier)
{
	member->identifier = identifier;
}


void
PrxMember_set_name(PrxMember *member, const char *name)
{
	snprintf(member->name, PRXMEMBER_NAME_LEN, "%s", name);
}


void
PrxMember_delete(PrxMember *member)
{
	free(member);
}
