/*
 * presagearchive.h
 *
 * This is the primary interface in order to interact with the two different
 * archive formats which come from Presage: (1) PRD/PRS pairs, and (2) PRX
 * archives.
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_PRESAGEARCHIVE_H
#define LUNAPURPURA_PRESAGEARCHIVE_H

#include "presagearchivemember.h"

/* Forward declarations. */
struct PRDPRS;
typedef struct PRDPRS PRDPRS;
struct PRX;
typedef struct PRX PRX;

enum PresageArchiveKind {
	PRESAGE_ARCHIVE_KIND_PRDPRS,
	PRESAGE_ARCHIVE_KIND_PRX,
};
typedef enum PresageArchiveKind PresageArchiveKind;

union PresageArchiveMetaContents {
	PRDPRS *prdprs;
	PRX *prx;
};

/*
 * Notice this is a tagged union.
 */
struct PresageArchive {
	PresageArchiveKind kind;
	union PresageArchiveMetaContents archive;
	uint16_t n_entries;
	uint32_t data_offset_start;
	PresageArchiveMember **members;
};
typedef struct PresageArchive PresageArchive;

PresageArchive *PresageArchive_NewFromFiles(const char *path1, const char *path2, LPStatus *status);
void PresageArchive_Close(PresageArchive *archive);
uint32_t PresageArchive_AbsoluteOffsetForMemberAtIndex(const PresageArchive *archive, const int index);
uint32_t PresageArchive_AbsoluteOffsetForMember(const PresageArchive *archive, const PresageArchiveMember *member);
PresageArchiveMember *PresageArchive_MemberWithResourceId(const PresageArchive *archive, char *filetype, uint32_t rid);
uint8_t *PresageArchive_DataForMember(const PresageArchive *archive, PresageArchiveMember *member);

#endif /* LUNAPURPURA_PRESAGEARCHIVE_H */
