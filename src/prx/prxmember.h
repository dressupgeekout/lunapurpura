/*
 * prx/prxmember.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_PRXMEMBER_H
#define LUNAPURPURA_PRXMEMBER_H

#include <lputil.h>

#include "prx.h"
#include "prxmember.h"

#define PRXMEMBER_FILETYPE_LEN 4
#define PRXMEMBER_NAME_LEN 16

#define PRXMEMBER_FILETYPE_AIF "Aif"
#define PRXMEMBER_FILETYPE_CLU "CLU"
#define PRXMEMBER_FILETYPE_SMF "SMF"
#define PRXMEMBER_FILETYPE_TXT "TXT"
#define PRXMEMBER_FILETYPE_XPK "XPK"

struct PRXMember {
	char filetype[PRXMEMBER_FILETYPE_LEN];
	uint16_t internal_id;
	uint32_t offset;
	uint32_t identifier;
	uint32_t size;
	char name[PRXMEMBER_NAME_LEN];
	void *data;
};
typedef struct PRXMember PRXMember;

PRXMember *PRXMember_New(LPStatus *status);
void PRXMember_SetInternalId(PRXMember *member, const uint16_t internal_id);
void PRXMember_SetOffset(PRXMember *member, const uint32_t offset);
void PRXMember_SetFiletype(PRXMember *member, const char filetype[PRXMEMBER_FILETYPE_LEN]);
void PRXMember_SetIdentifier(PRXMember *member, const uint32_t identifier);
void PRXMember_SetSize(PRXMember *member, const uint32_t size);
void PRXMember_SetName(PRXMember *member, const char *name);
void PRXMember_SetData(PRXMember *member, void *data);
void PRXMember_Free(PRXMember *member);

#endif /* LUNAPURPURA_PRXMEMBER_H */
