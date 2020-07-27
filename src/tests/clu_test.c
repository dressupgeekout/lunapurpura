/*
 * clu_test.c
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lputil.h>
#include <prx.h>
#include <presagearchive.h>
#include <clu.h>

#define MSGPREFIX "(CLU test)"

int
main(int argc, char *argv[])
{
	if (argc < 3) {
		LPWarn(MSGPREFIX, "usage: clu_test <prx> <rid>");
		return EXIT_FAILURE;
	}

	char *path = argv[1];
	uint32_t rid = (uint32_t)atoi(argv[2]);

	PresageArchiveMember *member = NULL;
	CLU *clu = NULL;

	LPStatus status;
	/* XXX PRX only for now */
	PresageArchive *archive = PresageArchive_NewFromFiles(path, NULL, &status);

	if (!archive) {
		LPWarn(MSGPREFIX, "couldn't read %s: %s", path, LPStatusString(status));
		if (status == LUNAPURPURA_CANTOPENFILE) {
			LPWarn(MSGPREFIX, "%s", strerror(errno));
		}
		goto fail;
	}

	member = PresageArchive_MemberWithResourceId(archive, "CLU", rid);

	if (!member) {
		LPWarn(MSGPREFIX, "couldn't find CLU-type PRX member with RID %ld", rid);
		goto fail;
	}

	uint32_t offset = PresageArchive_AbsoluteOffsetForMember(archive, member);
	fseek(archive->archive.prx->fp, (long)offset, SEEK_SET);
	clu = CLU_NewFromFile(archive->archive.prx->fp, &status);

	if (!clu) {
		LPWarn(MSGPREFIX, "couldn't create CLU: %s", LPStatusString(status));
		goto fail;
	}

	uint8_t *color = NULL;

	for (int i = 0; i < CLU_NELEMENTS; i++) {
		color = clu->array[i];
		LPWarn(MSGPREFIX, "%d\t(%d, %d, %d)", i, color[0], color[1], color[2]);
	}

	CLU_Free(clu);
	PresageArchive_Close(archive);
	return EXIT_SUCCESS;

fail:
	CLU_Free(clu);
	PresageArchive_Close(archive);
	return EXIT_FAILURE;
}
