/*
 * lperror
 */

#ifndef LUNAPURPURA_LPSTATUS_H
#define LUNAPURPURA_LPSTATUS_H

enum LPStatus {
	LUNAPURPURA_OK,
	LUNAPURPURA_ERROR,
	LUNAPURPURA_CANTOPENFILE,
	LUNAPURPURA_BADMAGIC,
};
typedef enum LPStatus LPStatus;

const char *LPStatusString(const LPStatus status);

#endif /* LUNAPURPURA_LPSTATUS_H */
