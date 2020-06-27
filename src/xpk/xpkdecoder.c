/*
 * xpk/xpkdecoder.c
 *
 * This file is part of Luna Purpura.
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef LUNAPURPURA_XPK_PNG_SUPPORT
#include <setjmp.h>
#include <png.h>
#endif

#include <lpstatus.h>
#include <lputil.h>

#include <clu.h>

#include "xpkdecoder.h"

static void xpk_decoder_init(XPKDecoder *decoder);

/* ********** */

static void
xpk_decoder_init(XPKDecoder *d)
{
	d->n_reps = 0;
	d->direct_counter = 0;
	d->next_holder = 0;
	d->repeat = 0;
	d->repeat_loc = 0L;
	d->line_repeat = 0;
	d->line_repeat_loc = 0L;
	d->cur_x = 0;
	d->cur_y = 0;
}

/* ********** */

#define NEWLINE(d)             \
	do {                         \
		d->cur_x = 0; d->cur_y++;  \
	} while (0)

#define SET_COLOR(rgba, rgba_i, r, g, b)  \
	do {                                    \
		rgba[rgba_i] = r; rgba_i++;           \
		rgba[rgba_i] = g; rgba_i++;           \
		rgba[rgba_i] = b; rgba_i++;           \
		rgba[rgba_i] = 0xFF; rgba_i++;        \
	} while (0)

/*
 * This function will allocate memory for you when successful; you'll need
 * to free the result yourself when you're done with it.
 */
uint8_t *
XPKDecoder_Decode(XPKDecoder *d, XPKEntry *entry, LPStatus *status)
{
	unsigned char byte = 0;
	unsigned char instruction = 0;
	unsigned char argument = 0;
	const uint8_t *color = NULL;
	unsigned int rgba_i = 0;
	size_t rv = 0;

	fseek(entry->xpk->file, entry->offset, SEEK_SET);

	xpk_decoder_init(d);

	/* Totally transparent RGBA matrix */
	uint8_t *rgba = calloc(entry->width * entry->height, 4 * sizeof(uint8_t));

	while (byte != XPKINST_EOF) {
		rv = ReadUint8(entry->xpk->file, 1, &byte);

		if (!rv) {
			warnx("couldn't ReadUint8");
			*status = LUNAPURPURA_ERROR;
			return NULL;
		}

#ifdef LUNAPURPURA_XPK_HACK
			/* XXX HACK!! */
			if (d->cur_y >= entry->height) {
				d->cur_y = entry->height - 1;
			}
#endif

		if (d->direct_counter > 0) {
			/* DIRECT MODE ENABLED */
			rgba_i = (d->cur_y * entry->width * 4) + (d->cur_x * 4);
			color = CLU_ColorAtIndex(entry->xpk->clu, byte);
#ifdef LUNAPURPURA_XPK_TRACE
			warnx("(writing color %d)", byte);
#endif
			SET_COLOR(rgba, rgba_i, color[0], color[1], color[2]);
			d->cur_x++;
			d->direct_counter--;
		} else {
			/* DIRECT MODE OFF OR EXPIRED */
			if (d->n_reps > 0) {
				/* RLE ENGAGED */
				rgba_i = (d->cur_y * entry->width * 4) + (d->cur_x * 4);
				color = CLU_ColorAtIndex(entry->xpk->clu, byte);
#ifdef LUNAPURPURA_XPK_TRACE
					warnx("(writing %d reps of color %d)", d->n_reps, byte);
#endif
				for (; d->n_reps > 0; d->n_reps--) {
					SET_COLOR(rgba, rgba_i, color[0], color[1], color[2]);
					d->cur_x++;
				}
			} else {
				/* NEED A NEW INSTRUCTION */
				instruction = byte >> 4; /* First 4 bits */
				argument = byte & 0x0F; /* Last 4 bits */

				switch (instruction) {
				case XPKINST_REPEAT:
					{
						switch (byte) {
						case XPKINST_RLENEXT:
							ReadUint8(entry->xpk->file, 1 , &byte);
							d->n_reps = byte;
#ifdef LUNAPURPURA_XPK_TRACE
							warnx("XPKINST_RLENEXT %d", byte);
#endif
							break;
						case XPKINST_REPEAT_END:
							{
								if (d->repeat > 0) {
									d->repeat--;
								}
								if (d->repeat > 0) {
									fseek(entry->xpk->file, d->repeat_loc, SEEK_SET);
#ifdef LUNAPURPURA_XPK_TRACE
									warnx("REPEAT_END (%d remaining)", d->repeat);
#endif
								} else {
									d->repeat_loc = 0L;
								}
							}
							break;
						default:
							d->repeat = argument;
							d->repeat_loc = ftell(entry->xpk->file);
#ifdef LUNAPURPURA_XPK_TRACE
						warnx("REPEAT %d", argument);
#endif
						}
					}
					break;
				case XPKINST_XSKIP:
					if (argument) {
						d->cur_x += argument;
#ifdef LUNAPURPURA_XPK_TRACE
						warnx("XSKIP %d", argument);
#endif
					} else {
						ReadUint16(entry->xpk->file, 1, &d->next_holder);
						d->cur_x += d->next_holder;
#ifdef LUNAPURPURA_XPK_TRACE
						warnx("XSKIP %d", d->next_holder);
#endif
					}
					break;
				case XPKINST_BIGXSKIP:
					d->cur_x += (16 + argument);
#ifdef LUNAPURPURA_XPK_TRACE
					warnx("BIGXSKIP %d", 16+argument);
#endif
					break;
				case XPKINST_RLE:
					/* 0x40 should be safe to ignore */
					/* 0x41 is already taken care of (XPKINST_EOF) */
					d->n_reps = argument;
#ifdef LUNAPURPURA_XPK_TRACE
					warnx("RLE %d", argument);
#endif
					break;
				case XPKINST_BIGRLE:
					d->n_reps = (16 + argument);
#ifdef LUNAPURPURA_XPK_TRACE
					warnx("BIGRLE %d", 16+argument);
#endif
					break;
				case XPKINST_DIRECT:
					d->direct_counter = argument;
#ifdef LUNAPURPURA_XPK_TRACE
					warnx("DIRECT %d", argument);
#endif
					break;
				case XPKINST_BIGDIRECT:
					d->direct_counter = (16 + argument);
#ifdef LUNAPURPURA_XPK_TRACE
					warnx("BIGDIRECT %d", 16+argument);
#endif
					break;
				case XPKINST_LINEREPEAT:
					{
						switch (byte) {
						case XPKINST_LINEREPEAT_END:
							{
								if (d->line_repeat > 0) {
									d->line_repeat--;
								}
								if (d->line_repeat > 0) {
									fseek(entry->xpk->file, d->line_repeat_loc, SEEK_SET);
								} else {
									d->line_repeat_loc = 0L;
								}
								NEWLINE(d);
#ifdef LUNAPURPURA_XPK_TRACE
								warnx("LINEREPEAT_END (%d remaining)", d->line_repeat);
#endif
								break;
							}
						default:
							NEWLINE(d);
							d->line_repeat = argument;
							d->line_repeat_loc = ftell(entry->xpk->file);
#ifdef LUNAPURPURA_XPK_TRACE
							warnx("LINEREPEAT %d", argument);
#endif
						}
					}
					break;
				case XPKINST_SETXNL:
					if (argument) {
						d->cur_x = argument;
#ifdef LUNAPURPURA_XPK_TRACE
						warnx("SETXNL %d", argument);
#endif
					} else {
						ReadUint16(entry->xpk->file, 1, &d->next_holder);
						d->cur_x = d->next_holder;
#ifdef LUNAPURPURA_XPK_TRACE
						warnx("SETXNL %d", d->next_holder);
#endif
					}
					d->cur_y++;
					break;
				case XPKINST_BIGSETXNL:
					d->cur_x = (16 + argument);
					d->cur_y++;
#ifdef LUNAPURPURA_XPK_TRACE
					warnx("BIGSETXNL %d", 16+argument);
#endif
					break;
				case XPKINST_RLENL:
					d->n_reps = argument;
					NEWLINE(d);
#ifdef LUNAPURPURA_XPK_TRACE
					warnx("RLENL %d", argument);
#endif
					break;
				case XPKINST_BIGRLENL:
					d->n_reps = (16 + argument);
					NEWLINE(d);
#ifdef LUNAPURPURA_XPK_TRACE
					warnx("BIGRLENL %d", 16+argument);
#endif
					break;
				case XPKINST_DIRECTNL:
					d->direct_counter = argument;
					NEWLINE(d);
#ifdef LUNAPURPURA_XPK_TRACE
					warnx("DIRECTNL %d", argument);
#endif
					break;
				case XPKINST_BIGDIRECTNL:
					d->direct_counter = (16 + argument);
					NEWLINE(d);
#ifdef LUNAPURPURA_XPK_TRACE
					warnx("BIGDIRECTNL %d", 16+argument);
#endif
					break;
				default:
					warnx("?? unhandled XPK instruction: %X", byte);
				}
			}
		}
	}

	*status = LUNAPURPURA_OK;
	return rgba;
}

#undef NEWLINE
#undef SET_COLOR

void
XPKDecoder_FreeRGBA(uint8_t *rgba)
{
	free(rgba);
}

#ifdef LUNAPURPURA_XPK_PNG_SUPPORT
LPStatus
XPKDecoder_RGBAToPNG(const uint8_t *rgba, const XPKEntry *entry, const char *path)
{
	/* XXX really should set up the error callbacks */
	png_structp pngwriter = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!pngwriter) {
		return LUNAPURPURA_ERROR;
	}

	png_infop pnginfo = png_create_info_struct(pngwriter);

	if (!pnginfo) {
		png_destroy_write_struct(&pngwriter, (png_infopp)NULL);
		return LUNAPURPURA_ERROR;
	}

	FILE *fp = fopen(path, "wb");

	if (!fp) {
		png_destroy_write_struct(&pngwriter, &pnginfo);
		return LUNAPURPURA_CANTOPENFILE;
	}

	png_init_io(pngwriter, fp);

	/*
	 * libpng's documention requests that we setjmp() here in case libpng
	 * crashes (it'll try to longjmp() back here in that event).
	 */
	if (setjmp(png_jmpbuf(pngwriter))) {
		png_destroy_write_struct(&pngwriter, &pnginfo);
		return LUNAPURPURA_ERROR;
	}

	png_set_IHDR(pngwriter, pnginfo, entry->width, entry->height, 8,
		PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	png_write_info(pngwriter, pnginfo);

	/*
	 * We've got a 1-dimensional array of RGBA values, but libpng prefers a
	 * 2-dimensional thing. Fortunately, there's a way around that.
	 */
	for (int i = 0; i < entry->height; i++) {
		png_bytep row = (png_bytep)&rgba[4*entry->width*i];
		png_write_row(pngwriter, row);
	}

	png_write_end(pngwriter, pnginfo);
	png_destroy_write_struct(&pngwriter, &pnginfo);

	fclose(fp);

	return LUNAPURPURA_OK;
}
#endif /* LUNAPURPURA_XPK_PNG_SUPPORT */
