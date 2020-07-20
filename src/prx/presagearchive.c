/*
 * This file is part of Luna Purpura.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lputil.h>

#include "presagearchive.h"
#include "presagearchivemember.h"
#include "prdprs.h"
#include "prx.h"

/*
 * If you want to extract a PRD/PRS pair, then `path1` represents the PRD
 * (header file) and `path2` represents the PRS (data file).
 *
 * If you want to extract a single PRX file, then indicate the path to it
 * with `path`, and set `path2` to NULL.
 */
PresageArchive *
PresageArchive_NewFromFiles(const char *path1, const char *path2, LPStatus *status)
{
	PresageArchive *archive = calloc(1, sizeof(PresageArchive));

	if (!archive) {
		*status = LUNAPURPURA_ERROR;
		goto fail;
	}

	if (path1 && path2) {
		archive->kind = PRESAGE_ARCHIVE_KIND_PRDPRS;
	} else {
		archive->kind = PRESAGE_ARCHIVE_KIND_PRX;
	}

	LPStatus new_archive_status;

	switch (archive->kind) {
	case PRESAGE_ARCHIVE_KIND_PRDPRS:
		{
			PRDPRS *prdprs = PRDPRS_New(path1, path2, &new_archive_status);
			if (new_archive_status != LUNAPURPURA_OK) {
				*status = new_archive_status;
				goto fail;
			}
			archive->archive.prdprs = prdprs;
		}
		break;
	case PRESAGE_ARCHIVE_KIND_PRX:
		{
			PRX *prx = PRX_New(path1, &new_archive_status);
			if (new_archive_status != LUNAPURPURA_OK) {
				*status = new_archive_status;
				goto fail;
			}
			archive->archive.prx = prx;
		}
		break;
	}

	/*
	 * At this stage in the process, we are only concerned about the members'
	 * metadata, so, obtain the appropriate file pointer for that task.
	 */
	FILE *fp;
	switch (archive->kind) {
		case PRESAGE_ARCHIVE_KIND_PRDPRS: fp = archive->archive.prdprs->header_fp; break;
		case PRESAGE_ARCHIVE_KIND_PRX: fp = archive->archive.prx->fp; break;
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
	 * present at the start and/or end of the archive's table of contents.
	 */
	uint16_t actual_n_entries;
	ReadUint16LE(fp, 1, &actual_n_entries);

	/* This is the real count of entries we ultimately care about. */
	ReadUint16LE(fp, 1, &archive->n_entries);

	fseek(fp, 2L, SEEK_CUR); /* Ignore */

	archive->members = calloc(archive->n_entries, sizeof(PresageArchiveMember));

	/*
	 * Purposefully skip over the zeroeth entry (which has internal_id of 1),
	 * it's always a "dummy" entry which serves little purpose.
	 */
	fseek(fp, 24L, SEEK_CUR);

	/*
	 * OK now we *finally* can access the individual archive members.
	 */
	for (int i = 0; i < archive->n_entries; i++) {
		LPStatus new_member_status;
		PresageArchiveMember *member = PresageArchiveMember_New(&new_member_status);

		if (new_member_status != LUNAPURPURA_OK) {
			*status = new_member_status;
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
			PresageArchiveMember_SetInternalId(member, archive->n_entries+1);
		} else {
			PresageArchiveMember_SetInternalId(member, internal_id);
		}

		fseek(fp, 6L, SEEK_CUR);

		uint32_t offset;
		ReadUint32LE(fp, 1, &offset);
		PresageArchiveMember_SetOffset(member, offset);

		PresageArchiveMemberFiletype filetype;
		ReadChar(fp, sizeof(PresageArchiveMemberFiletype), filetype);
		PresageArchiveMember_SetFiletype(member, filetype);

		uint32_t rid;
		ReadUint32LE(fp, 1, &rid);
		PresageArchiveMember_SetResourceId(member, rid);

		uint32_t size;
		ReadUint32LE(fp, 1, &size);
		PresageArchiveMember_SetSize(member, size);

		archive->members[i] = member;
	}

	/*
	 * Account for the dummy entries at the end of the table of contents, if
	 * necessary.
	 */
	fseek(fp, 24L*(actual_n_entries - archive->n_entries), SEEK_CUR);

	/*
	 * In PRXes, the "offset" field for each member does NOT indicate an absolute
	 * offset. It's actually an offset from the position of the file we're
	 * currently at right now. So let's keep track of it.
	 */
	if (archive->kind == PRESAGE_ARCHIVE_KIND_PRX) {
		archive->data_offset_start = ftell(fp);
	}

	/*
	 * OK now let's figure out the individual member names. We start with the
	 * 1st member (not the 0th member) because remember we actually can't
	 * obtain information about the "dummy" entry at the beginning.
	 *
	 * Also, the offset brings us to the start of the data, but that skips
	 * past the filename!  So that's why we rewind a little.
	 *
	 * This is a little tricky because, in PRD/PRS pairs, the member names are
	 * NOT stored in the header. So we have to bring the other file pointer into
	 * the picture now.
	 */
	FILE *name_finder_fp = NULL;

	switch (archive->kind) {
		case PRESAGE_ARCHIVE_KIND_PRDPRS: name_finder_fp = archive->archive.prdprs->data_fp; break;
		case PRESAGE_ARCHIVE_KIND_PRX: name_finder_fp = archive->archive.prx->fp; break;
	}

	for (int i = 0; i < archive->n_entries; i++) {
		uint32_t abs_offset = PresageArchive_AbsoluteOffsetForMemberAtIndex(archive, i);
		fseek(name_finder_fp, abs_offset, SEEK_SET);

		/* 
		 * Rewind to get the filename. Purposefully avoid an extra underscore in
		 * PRD/PRS members.
		 */
		long rewind_amt = PRESAGEARCHIVEMEMBER_NAME_LEN;
		rewind_amt += (archive->kind == PRESAGE_ARCHIVE_KIND_PRDPRS) ? 5 : 6;
		fseek(name_finder_fp, -rewind_amt, SEEK_CUR); 

		PresageArchiveMemberName name;
		ReadChar(name_finder_fp, sizeof(PresageArchiveMemberName), name);
		PresageArchiveMember_SetName(archive->members[i], name);
	}

	*status = LUNAPURPURA_OK;
	return archive;

fail:
	PresageArchive_Close(archive);
	return NULL;
}


/*
 * This will also free all of the individual archive members, too, but make
 * sure to familiarize yourself with the caveats about that.
 */
void
PresageArchive_Close(PresageArchive *archive)
{
	if (!archive) return;

	switch (archive->kind) {
	case PRESAGE_ARCHIVE_KIND_PRDPRS:
		PRDPRS_Close(archive->archive.prdprs);
		break;
	case PRESAGE_ARCHIVE_KIND_PRX:
		PRX_Close(archive->archive.prx);
		break;
	}

	for (int i = 0; i < archive->n_entries; i++) {
		PresageArchiveMember_Free(archive->members[i]);
	}

	free(archive);
}


uint32_t
PresageArchive_AbsoluteOffsetForMemberAtIndex(const PresageArchive *archive, const int index)
{
	switch (archive->kind) {
	case PRESAGE_ARCHIVE_KIND_PRDPRS:
		return archive->members[index]->offset;
		break;
	case PRESAGE_ARCHIVE_KIND_PRX:
		return archive->data_offset_start + archive->members[index]->offset;
		break;
	}
}


uint32_t
PresageArchive_AbsoluteOffsetForMember(const PresageArchive *archive, const PresageArchiveMember *member)
{
	switch (archive->kind) {
	case PRESAGE_ARCHIVE_KIND_PRDPRS:
		return member->offset;
		break;
	case PRESAGE_ARCHIVE_KIND_PRX:
		return archive->data_offset_start + member->offset;
		break;
	}
}


/*
 * Returns the PresageArchiveMember with the given filetype and RID. This does
 * not actually return the data.
 *
 * XXX This traversal could be pretty slow
 *
 * May return NULL.
 */
PresageArchiveMember *
PresageArchive_MemberWithResourceId(const PresageArchive *archive, char *filetype, uint32_t rid)
{
	PresageArchiveMember *member = NULL;

	for (int i = 0; i < archive->n_entries; i++) {
		member = archive->members[i];
		if (!strncmp(member->filetype, filetype, PRESAGEARCHIVEMEMBER_FILETYPE_LEN)) {
			if (member->rid == rid) {
				return member;
			}
		}
	}

	return NULL;
}


uint8_t *
PresageArchive_DataForMember(const PresageArchive *archive, PresageArchiveMember *member)
{
	uint8_t *data = calloc(member->size, 1);

	if (!data) {
		return NULL;
	}

	uint32_t abs_offset = PresageArchive_AbsoluteOffsetForMember(archive, member);

	FILE *data_fp;

	switch (archive->kind) {
	case PRESAGE_ARCHIVE_KIND_PRDPRS:
		data_fp = archive->archive.prdprs->data_fp;
		break;
	case PRESAGE_ARCHIVE_KIND_PRX:
		data_fp = archive->archive.prx->fp;
		break;
	}

	fseek(data_fp, abs_offset, SEEK_SET);
	fread(data, 1, member->size, data_fp);
	PresageArchiveMember_SetData(member, data);

	return member->data;
}
