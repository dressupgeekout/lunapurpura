/*
 * prx/prxmember.c
 *
 * This file is part of Luna Purpura.
 */

#include <stdio.h>
#include <stdlib.h>

#include <lpstatus.h>

#include "prx.h"

PRXMember *
PRXMember_New(LPStatus *status)
{
	PRXMember *member = calloc(1, sizeof(PRXMember));

	if (!member) {
		*status = LUNAPURPURA_ERROR;
		return NULL;
	}

	member->data = NULL;

	*status = LUNAPURPURA_OK;
	return member;
}


void
PRXMember_SetInternalId(PRXMember *member, const uint16_t internal_id)
{
	member->internal_id = internal_id;
}


void
PRXMember_SetOffset(PRXMember *member, const uint32_t offset)
{
	member->offset = offset;
}


void
PRXMember_SetFiletype(PRXMember *member, const char *filetype)
{
	snprintf(member->filetype, PRXMEMBER_FILETYPE_LEN, "%s", filetype);
}


void
PRXMember_SetIdentifier(PRXMember *member, const uint32_t identifier)
{
	member->identifier = identifier;
}


void
PRXMember_SetSize(PRXMember *member, const uint32_t size)
{
	member->size = size;
}


void
PRXMember_SetName(PRXMember *member, const char *name)
{
	snprintf(member->name, PRXMEMBER_NAME_LEN, "%s", name);
}


/*
 * Ownership of the data is taken over, so don't free the data yourself.
 * PRXMember_Free() will take care of it.
 *
 * You really should have already set the size of the data with
 * PRXMember_SetSize().
 */
void
PRXMember_SetData(PRXMember *member, void *data)
{
	member->data = data;
}


/*
 * NOTE: This frees the member->data for you, too. Read the notes for
 * PRXMember_SetData() for more info.
 */
void
PRXMember_Free(PRXMember *member)
{
	free(member->data);
	free(member);
}
