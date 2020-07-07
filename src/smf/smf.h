/*
 * smf/smf.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_SMF_H
#define LUNAPURPURA_SMF_H

#include <lputil.h>

#define LP_SUBSYSTEM_SMF "smf"

#define SMF_MAGIC_LEN 4

extern uint8_t SMF_MAGIC[SMF_MAGIC_LEN];

struct SMF {
	void* ignore;
};
typedef struct SMF SMF;

SMF *SMF_NewFromFile(const char *path, LPStatus *status);
void SMF_Free(SMF *smf);

#endif /* LUNAPURPURA_SMF_H */
