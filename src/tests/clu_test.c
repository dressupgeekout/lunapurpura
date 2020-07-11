/*
 * clu_test.c
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lputil.h>
#include <prx.h>
#include <prxmember.h>
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

	PRXMember *member = NULL;
	CLU *clu = NULL;

	LPStatus status;
	PRX *prx = PRX_NewFromFile(path, true, &status);

	if (!prx) {
		LPWarn(MSGPREFIX, "couldn't read %s: %s", path, LPStatusString(status));
		if (status == LUNAPURPURA_CANTOPENFILE) {
			LPWarn(MSGPREFIX, "%s", strerror(errno));
		}
		goto fail;
	}

	member = PRX_MemberWithResourceId(prx, "CLU", rid);

	if (!member) {
		LPWarn(MSGPREFIX, "couldn't find PRX member %ld", rid);
		goto fail;
	}

	clu = CLU_NewFromData(member->data, &status);

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
	PRX_Free(prx);
	return EXIT_SUCCESS;

fail:
	CLU_Free(clu);
	PRX_Free(prx);
	return EXIT_FAILURE;
}
