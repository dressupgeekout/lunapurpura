/*
 * presagearchivemember.c
 *
 * This file is part of Luna Purpura.
 */

#include <stdio.h>
#include <stdlib.h>

#include <lputil.h>

#include "presagearchivemember.h"

PresageArchiveMember *
PresageArchiveMember_New(LPStatus *status)
{
	PresageArchiveMember *member = calloc(1, sizeof(PresageArchiveMember));

	if (!member) {
		*status = LUNAPURPURA_ERROR;
		return NULL;
	}

	member->data = NULL;

	*status = LUNAPURPURA_OK;
	return member;
}


void
PresageArchiveMember_SetInternalId(PresageArchiveMember *member, const uint16_t internal_id)
{
	member->internal_id = internal_id;
}


void
PresageArchiveMember_SetOffset(PresageArchiveMember *member, const uint32_t offset)
{
	member->offset = offset;
}


void
PresageArchiveMember_SetFiletype(PresageArchiveMember *member, const char *filetype)
{
	snprintf(member->filetype, sizeof(member->filetype), "%s", filetype);
}


void
PresageArchiveMember_SetResourceId(PresageArchiveMember *member, const uint32_t rid)
{
	member->rid = rid;
}


void
PresageArchiveMember_SetSize(PresageArchiveMember *member, const uint32_t size)
{
	member->size = size;
}


void
PresageArchiveMember_SetName(PresageArchiveMember *member, const char *name)
{
	snprintf(member->name, sizeof(member->name), "%s", name);
}


/**
 * Ownership of the data is taken over, so don't free the data yourself.
 * PresageArchiveMember_FreeData() will take care of it.
 *
 * You really should have already set the size of the data with
 * PresageArchiveMember_SetSize().
 */
void
PresageArchiveMember_SetData(PresageArchiveMember *member, uint8_t *data)
{
	member->data = data;
}


/**
 * NOTE: This does NOT free the member->data for you, too. Read the notes
 * for PresageArchiveMember_SetData() for more info.
 */
void
PresageArchiveMember_Free(PresageArchiveMember *member)
{
	if (!member) return;
	free(member);
}


/**
 * This ONLY frees the member's data. It does NOT free the member struct
 * itself. The purpose of this is to save memory when you want the actual
 * data, but still have access to all metadata in case we need it.
 */
void
PresageArchiveMember_FreeData(PresageArchiveMember *member)
{
	if (!member) return;
	if (member->data) free(member->data);
}
