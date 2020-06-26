/*
 * prx/prx.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_PRX_H
#define LUNAPURPURA_PRX_H

#include <stdint.h>
#include <stdio.h>

#define PRX_HEADER_MAGIC '1'

#define PRXMEMBER_FILETYPE_LEN 4
#define PRXMEMBER_NAME_LEN 16

#define PRXMEMBER_FILETYPE_AIF "Aif"
#define PRXMEMBER_FILETYPE_CLU "CLU"
#define PRXMEMBER_FILETYPE_SMF "SMF"
#define PRXMEMBER_FILETYPE_TXT "TXT"
#define PRXMEMBER_FILETYPE_XPK "XPK"

typedef struct PrxMember PrxMember;

struct PrxHeader {
	char magic;
	uint8_t *__unknown1;
	uint16_t n_members;
	uint16_t __unknown2;
	uint16_t __unknown3;
};
typedef struct PrxHeader PrxHeader;

struct PrxFile {
	PrxHeader *header;
	PrxMember **members;
};
typedef struct PrxFile PrxFile;

/* ****** */

PrxHeader *PrxHeader_new(size_t n_members);
PrxHeader *PrxHeader_read(FILE *fp);
size_t PrxHeader_write(const PrxHeader *header, FILE *fp);
void PrxHeader_delete(PrxHeader *header);

/* ****** */

PrxFile *PrxFile_new(const PrxHeader *header);
PrxFile *PrxFile_read(FILE *fp);
void PrxFile_add_member(PrxFile *prxfile, PrxMember *member);
void PrxFile_delete(PrxFile *prxfile);

#endif /* LUNAPURPURA_PRX_H */
