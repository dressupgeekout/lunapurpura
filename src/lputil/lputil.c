/*
 * lputil/lputil.c
 *
 * This file is part of Luna Purpura.
 */

#include <stdarg.h>
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

/* ********** */

/* PRIVATE */
static void lp_msg_write(FILE *stream, const char *subsystem, const char *fmt, va_list ap);

/* ********** */

static void
lp_msg_write(FILE *stream, const char *subsystem, const char *fmt, va_list ap)
{
	static char real_fmt[512];
	snprintf(real_fmt, sizeof(real_fmt), "%s: %s\n", subsystem, fmt);
	vfprintf(stream, real_fmt, ap);
	va_end(ap);
}

/* ********** */

/*
 * Unconditionally writes the formatted message to the standard output.
 */
void
LPLog(const char *subsystem, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	lp_msg_write(stdout, subsystem, fmt, ap);
}


/*
 * Unconditionally writes the formatted message to the standard error.
 */
void
LPWarn(const char *subsystem, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	lp_msg_write(stderr, subsystem, fmt, ap);
}


/*
 * Prints a formatted message to stderr, but only if debug support is
 * enabled in the first place.
 */
void
LPDebug(const char *subsystem, const char *fmt, ...)
{
#ifdef LUNAPURPURA_DEBUG
	va_list ap;
	va_start(ap, fmt);
	lp_msg_write(stderr, subsystem, fmt, ap);
#endif
}


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


bool
ValidateMagic(uint8_t *region, const uint8_t *expected_magic, const size_t magic_len)
{
	uint8_t *regionptr = region;

	for (int i = 0; i < magic_len; i++) {
		if (*regionptr == expected_magic[i]) {
			regionptr++;
		} else {
			LPDebug("", "(%s) %d != %d", __func__, *regionptr, expected_magic[i]);
			return false;
		}
	}

	return true;
}


/*
 * The FILE pointer is expected to have already been rewinded to the
 * beginning of the file. If the magic is good, then we return true and
 * leave the file seeked to the end of the magic. Otherwise, the magic is
 * bad and we return false and close the file.
 */
bool
ValidateMagicF(FILE *file, const uint8_t *expected_magic, const size_t magic_len)
{
	bool rv = true;

	uint8_t *our_magic = malloc(magic_len);
	if (!our_magic) {
		LPWarn("", "(%s) Couldn't malloc our magic", __func__);
		fclose(file);
		rv = false;
		goto done;
	}

	fread(our_magic, 1, magic_len, file);

	for (int i = 0; i < magic_len; i++) {
		if (our_magic[i] != expected_magic[i]) {
			LPDebug("", "(%s) %d != %d", __func__, our_magic[i], expected_magic[i]);
			fclose(file);
			rv = false;
			goto done;
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
#ifndef LUNAPURPURA_BIG_ENDIAN
	*dest = htons(*dest);
#endif
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
#ifdef LUNAPURPURA_BIG_ENDIAN
	*dest = ntohs(*dest);
#endif
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
#ifndef LUNAPURPURA_BIG_ENDIAN
	*dest = htonl(*dest);
#endif
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
#ifdef LUNAPURPURA_BIG_ENDIAN
	*dest = ntohl(*dest);
#endif
	return rv;
}
