/*
* prx/prx.c
*
* This file is part of Luna Purpura.
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lputil.h>

#include "prx.h"
#include "prxmember.h"

PRX *
PRX_NewFromFile(const char *path, bool want_data, LPStatus *status)
{
	PRX *prx = calloc(1, sizeof(PRX));

	if (!prx) {
		*status = LUNAPURPURA_ERROR;
		return NULL;
	}

	FILE *fp = fopen(path, "rb");

	if (!fp) {
		*status = LUNAPURPURA_CANTOPENFILE;
		goto fail;
	}

	uint8_t magic;
	ReadUint8(fp, 1, &magic);
	if (magic != PRX_MAGIC) {
		*status = LUNAPURPURA_BADMAGIC;
		goto fail;
	}

	/* 128 + 10 - 1, I guess -- all zeroes */
	fseek(fp, 137L, SEEK_CUR);

	/*
	 * This is the number of entries _including_ dummy ones, which may be
	 * present at the start and/or end of the PRX's table of contents.
	 */
	uint16_t actual_n_entries;
	ReadUint16LE(fp, 1, &actual_n_entries);

	/* This is the real count of entries we ultimately care about. */
	ReadUint16LE(fp, 1, &prx->n_entries);

	fseek(fp, 2L, SEEK_CUR); /* Ignore */

	prx->members = calloc(prx->n_entries, sizeof(PRXMember));

	/*
	 * Purposefully skip over the zeroeth entry (which has internal_id of 1),
	 * it's always a "dummy" entry which serves little purpose.
	 */
	fseek(fp, 24L, SEEK_CUR);

	/*
	 * OK now we *finally* can access the individual archive members.
	 */
	for (int i = 0; i < prx->n_entries; i++) {
		LPStatus new_member_status;
		PRXMember *member = PRXMember_New(&new_member_status);

		if (new_member_status != LUNAPURPURA_OK) {
			*status = LUNAPURPURA_ERROR;
			goto fail;
		}

		/*
		 * The last internal_id is always stored as '0' rather than what you'd
		 * expect. Anthony Kozar suspects this is to merely signify the end of the
		 * list, since we already know how many members there are by now.
		 */
		uint16_t internal_id;
		ReadUint16LE(fp, 1, &internal_id);
		if (internal_id == 0) {
			PRXMember_SetInternalId(member, prx->n_entries+1);
		} else {
			PRXMember_SetInternalId(member, internal_id);
		}

		fseek(fp, 6L, SEEK_CUR);

		uint32_t offset;
		ReadUint32LE(fp, 1, &offset);
		PRXMember_SetOffset(member, offset);

		char filetype[PRXMEMBER_FILETYPE_LEN];
		ReadChar(fp, PRXMEMBER_FILETYPE_LEN, filetype);
		PRXMember_SetFiletype(member, filetype);

		uint32_t rid;
		ReadUint32LE(fp, 1, &rid);
		PRXMember_SetResourceId(member, rid);

		uint32_t size;
		ReadUint32LE(fp, 1, &size);
		PRXMember_SetSize(member, size);

		prx->members[i] = member;
	}

	/*
	 * Account for the dummy entries at the end of the table of contents, if
	 * necessary.
	 */
	fseek(fp, 24L*(actual_n_entries - prx->n_entries), SEEK_CUR);

	/*
	 * The "offset" field for each member does NOT indicate an absolute
	 * offset within this PRX file. It's actually an offset from the position
	 * of the file we're currently at right now. So let's keep track of it.
	 */
	prx->data_offset_start = ftell(fp);

	/*
	 * OK now let's figure out the individual member names. We start with the
	 * 1st member (not the 0th member) because remember we actually can't
	 * obtain information about the "dummy" entry at the beginning.
	 *
	 * Also, the offset brings us to the start of the data, but that skips
	 * past the filename!  So that's why we rewind a little.
	 */
	for (int i = 0; i < prx->n_entries; i++) {
		uint32_t abs_offset = PRX_AbsoluteOffsetForMember(prx, i);
		fseek(fp, abs_offset, SEEK_SET);
		fseek(fp, (long)-(PRXMEMBER_NAME_LEN+6), SEEK_CUR); /* Rewind to get the filename */

		char name[PRXMEMBER_NAME_LEN];
		ReadChar(fp, PRXMEMBER_NAME_LEN, name);
		PRXMember_SetName(prx->members[i], name);

		if (want_data) {
			/*
			 * XXX We're loading the entire member into memory. This ought to be
			 * chunked instead, for less powerful platforms.
			 */
			void *data = calloc(prx->members[i]->size, 1);
			if (!data) {
				*status = LUNAPURPURA_ERROR;
				goto fail;
			}

			fseek(fp, abs_offset, SEEK_SET);
			fread(data, 1, prx->members[i]->size, fp);

			PRXMember_SetData(prx->members[i], data);
		}
	}

	/* Success! */
	fclose(fp);
	*status = LUNAPURPURA_OK;
	return prx;

fail:
		PRX_Free(prx);
		if (fp) fclose(fp);
		return NULL;
}


void
PRX_Free(PRX *prx)
{
	if (!prx) {
		return;
	}

	for (int i = 0; i < prx->n_entries; i++) {
		PRXMember_Free(prx->members[i]);
	}
	free(prx);
}


uint32_t
PRX_AbsoluteOffsetForMember(const PRX *prx, const int index)
{
	return prx->data_offset_start + prx->members[index]->offset;
}


/*
 * XXX This traversal could be pretty slow
 *
 * May return NULL.
 */
PRXMember *
PRX_MemberWithResourceId(const PRX *prx,  char *filetype, uint32_t rid)
{
	PRXMember *member = NULL;

	for (int i = 0; i < prx->n_entries; i++) {
		member = prx->members[i];
		if (!strncmp(member->filetype, filetype, PRXMEMBER_FILETYPE_LEN)) {
			if (member->rid == rid) {
				return member;
			}
		}
	}

	return NULL;
}
