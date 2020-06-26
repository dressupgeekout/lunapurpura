/*
 * lputil/lputil.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_UTIL_H
#define LUNAPURPURA_UTIL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

bool ValidateMagic(FILE *file, const uint8_t *expected_magic, const size_t magic_len);
size_t ReadUint8(FILE *file, size_t count, uint8_t *dest);
size_t ReadUint16(FILE *file, size_t count, uint16_t *dest);
size_t ReadUint32(FILE *file, size_t count, uint32_t *dest);

#endif /* LUNAPURPURA_UTIL_H */
