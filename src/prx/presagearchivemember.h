/*
 * prx/prxmember.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_PRESAGEARCHIVEMEMBER_H
#define LUNAPURPURA_PRESAGEARCHIVEMEMBER_H

#include <lputil.h>

#include "prx.h"

#define PRXMEMBER_FILETYPE_AIF "Aif"
#define PRXMEMBER_FILETYPE_CLU "CLU"
#define PRXMEMBER_FILETYPE_SMF "SMF"
#define PRXMEMBER_FILETYPE_TXT "TXT"
#define PRXMEMBER_FILETYPE_XPK "XPK"

#define PRESAGEARCHIVEMEMBER_FILETYPE_LEN 4
typedef char PresageArchiveMemberFiletype[PRESAGEARCHIVEMEMBER_FILETYPE_LEN];

#define PRESAGEARCHIVEMEMBER_NAME_LEN 16
typedef char PresageArchiveMemberName[PRESAGEARCHIVEMEMBER_NAME_LEN];

struct PresageArchiveMember {
	PresageArchiveMemberFiletype filetype;
	uint16_t internal_id;
	uint32_t offset;
	uint32_t rid;
	uint32_t size;
	PresageArchiveMemberName name;
	uint8_t *data;
};
typedef struct PresageArchiveMember PresageArchiveMember;

PresageArchiveMember *PresageArchiveMember_New(LPStatus *status);
void PresageArchiveMember_SetInternalId(PresageArchiveMember *member, const uint16_t internal_id);
void PresageArchiveMember_SetOffset(PresageArchiveMember *member, const uint32_t offset);
void PresageArchiveMember_SetFiletype(PresageArchiveMember *member, const char filetype[PRESAGEARCHIVEMEMBER_FILETYPE_LEN]);
void PresageArchiveMember_SetResourceId(PresageArchiveMember *member, const uint32_t rid);
void PresageArchiveMember_SetSize(PresageArchiveMember *member, const uint32_t size);
void PresageArchiveMember_SetName(PresageArchiveMember *member, const char *name);
void PresageArchiveMember_SetData(PresageArchiveMember *member, uint8_t *data);
void PresageArchiveMember_Free(PresageArchiveMember *member);
void PresageArchiveMember_FreeData(PresageArchiveMember *member);

#endif /* LUNAPURPURA_PRESAGEARCHIVEMEMBER_H */
