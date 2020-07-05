/*
 * lputil/lputil.c
 *
 * This file is part of Luna Purpura.
 */

#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef LUNAPURPURA_TARGET_WINDOWS
#include "getopt.h" /* For getopt(3) */
#include <WinSock2.h> /* For htons(3) et al */
#else
#include <unistd.h> /* For getopt(3) */
#include <arpa/inet.h> /* For htons(3) et al */
#endif

#include "lputil.h"


int
LPGetopt(int nargc, char * const *nargv, const char *options)
{
	return getopt(nargc, nargv, options);
}


/*
 * Returns a string representation of the given LPStatus.
 */
const char *
LPStatusString(const LPStatus status)
{
	switch (status) {
		case LUNAPURPURA_OK: return "OK"; break;
		case LUNAPURPURA_ERROR: return "Error"; break;
		case LUNAPURPURA_CANTOPENFILE: return "Can't open file"; break;
		case LUNAPURPURA_BADMAGIC: return "Bad magic"; break;
		default: return "";
	}
}


/*
 * The FILE pointer is expected to have already been rewinded to the
 * beginning of the file. If the magic is good, then we return true and
 * leave the file seeked to the end of the magic. Otherwise, the magic is
 * bad and we return false and close the file.
 */
bool
ValidateMagic(FILE *file, const uint8_t *expected_magic, const size_t magic_len)
{
	bool rv = true;

	uint8_t *our_magic = malloc(magic_len);
	if (!our_magic) {
		warn("(%s) Couldn't malloc our magic", __func__);
		fclose(file);
		rv = false;
		goto done;
	}

	fread(our_magic, 1, magic_len, file);

	for (int i = 0; i < magic_len; i++) {
		if (our_magic[i] != expected_magic[i]) {
#ifdef LUNAPURPURA_DEBUG
			warnx("(%s) %d != %d", __func__, our_magic[i], expected_magic[i]);
#endif
			fclose(file);
			rv = false;
			goto done;
		} else {
#ifdef LUNAPURPURA_DEBUG
			warnx("(%s) %d == %d", __func__, our_magic[i], expected_magic[i]);
#endif
		}
	}

done:
	free(our_magic);
	return rv;
}


/*
 * Reads `count` entries of signed 8-bit values from the `file`, and writes
 * them to `dest`. Returns the count.
 */
size_t
ReadChar(FILE *file, size_t count, char *dest)
{
	return fread(dest, sizeof(char), count, file);
}


/*
 * Reads `count` entries of unsigned 8-bit values from the `file`, and writes
 * them to `dest`. Returns the count.
 */
size_t
ReadUint8(FILE *file, size_t count, uint8_t *dest)
{
	return fread(dest, sizeof(uint8_t), count, file);
}


/*
 * Reads `count` entries of unsigned *big-endian* 16-bit values from the
 * `file`, and writes them to `dest`. Returns the count.
 */
size_t
ReadUint16(FILE *file, size_t count, uint16_t *dest)
{
	size_t rv = fread(dest, sizeof(uint16_t), count, file);
	*dest = htons(*dest);
	return rv;
}


/*
 * Reads `count` entries of unsigned *little-endian* 16-bit values from the
 * `file`, and writes them to `dest`. Returns the count.
 */
size_t
ReadUint16LE(FILE *file, size_t count, uint16_t *dest)
{
	size_t rv = fread(dest, sizeof(uint16_t), count, file);
	/* XXX do something to guarantee it was little-endian */
	return rv;
}


/*
 * Reads `count` entries of unsigned *big-endian* 32-bit values from the
 * `file`, and writes them to `dest`. Returns the count.
 */
size_t
ReadUint32(FILE *file, size_t count, uint32_t *dest)
{
	size_t rv = fread(dest, sizeof(uint32_t), count, file);
	*dest = htonl(*dest);
	return rv;
}


/*
 * Reads `count` entries of unsigned *little-endian* 32-bit values from the
 * `file`, and writes them to `dest`. Returns the count.
 */
size_t
ReadUint32LE(FILE *file, size_t count, uint32_t *dest)
{
	size_t rv = fread(dest, sizeof(uint32_t), count, file);
	/* XXX do something to guarantee it was little-endian */
	return rv;
}
