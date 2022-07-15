/*
 * xpk/xpkdecoder.c
 *
 * This file is part of Luna Purpura.
 */

#include <stdlib.h>
#include <stdio.h>

#ifdef LUNAPURPURA_PNG_SUPPORT
#include <lpstb_image_write.h>
#endif

#include <lputil.h>

#include <clu.h>

#include "xpkdecoder.h"

static struct RepeatMarker *new_repeat_marker(void);
static void xpk_decoder_init(XPKDecoder *decoder);
static void push_repeat_marker(XPKDecoder *decoder, bool with_newline, unsigned int reps, long loc);
static void pop_repeat_marker(XPKDecoder *decoder);
static struct RepeatMarker *current_repeat_marker(XPKDecoder *decoder);

/* ********** */

/*
 * RepeatMarkers are allocated with push_repeat_marker() and are deallocated
 * with pop_repeat_marker().
 */
static struct RepeatMarker *
new_repeat_marker(void)
{
	struct RepeatMarker *marker = calloc(1, sizeof(struct RepeatMarker));
	marker->newline = false;
	marker->reps = 0;
	marker->loc = 0L;
	return marker;
}


static void
xpk_decoder_init(XPKDecoder *d)
{
	d->n_reps = 0;
	d->direct_counter = 0;
	d->next_holder = 0;
	d->cur_x = 0;
	d->cur_y = 0;

	for (int i = 0; i < XPK_MAX_REPEAT_MARKERS; i++) {
		d->repeat_markers[i] = NULL;
	}

	d->repeat_marker_top = -1;
}


static void
push_repeat_marker(XPKDecoder *d, bool with_newline, unsigned int reps, long loc)
{
#ifdef LUNAPURPURA_XPK_TRACE
	LPWarn(LP_SUBSYSTEM_XPK, "pushing new repeat marker");
#endif
	d->repeat_marker_top++;
	if (d->repeat_marker_top >= XPK_MAX_REPEAT_MARKERS) {
		LPWarn(LP_SUBSYSTEM_XPK, "%s: STACK OVERFLOW!!", __func__);
		d->repeat_marker_top--;
	} else {
		d->repeat_markers[d->repeat_marker_top] = new_repeat_marker();
		d->repeat_markers[d->repeat_marker_top]->newline = with_newline;
		d->repeat_markers[d->repeat_marker_top]->reps = reps;
		d->repeat_markers[d->repeat_marker_top]->loc = loc;
	}
}


static void
pop_repeat_marker(XPKDecoder *d)
{
#ifdef LUNAPURPURA_XPK_TRACE
	LPWarn(LP_SUBSYSTEM_XPK, "popping repeat marker");
#endif
	free(d->repeat_markers[d->repeat_marker_top]);
	d->repeat_marker_top--;
}


static struct RepeatMarker *
current_repeat_marker(XPKDecoder *d)
{
	return d->repeat_markers[d->repeat_marker_top];
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

	while (!(byte == XPKINST_EOF && d->n_reps)) {
		rv = ReadUint8(entry->xpk->file, 1, &byte);

		if (!rv) {
			LPWarn(LP_SUBSYSTEM_XPK, "couldn't ReadUint8");
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
			LPWarn(LP_SUBSYSTEM_XPK, "(writing color %d)", byte);
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
					LPWarn(LP_SUBSYSTEM_XPK, "(writing %d reps of color %d)", d->n_reps, byte);
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
							LPWarn(LP_SUBSYSTEM_XPK, "RLENEXT %d", byte);
#endif
							break;
						case XPKINST_REPEAT_END:
							{
								struct RepeatMarker *marker = current_repeat_marker(d);
								if (marker->reps > 0) {
											 marker->reps--;
								}
								if (marker->reps > 0) {
									fseek(entry->xpk->file, marker->loc, SEEK_SET);
#ifdef LUNAPURPURA_XPK_TRACE
									LPWarn(LP_SUBSYSTEM_XPK, "REPEAT_END (%d remaining)", marker->reps);
#endif
								} else {
									pop_repeat_marker(d);
								}
							}
							break;
						default:
							push_repeat_marker(d, false, argument, ftell(entry->xpk->file));
#ifdef LUNAPURPURA_XPK_TRACE
							LPWarn(LP_SUBSYSTEM_XPK, "REPEAT %d", argument);
#endif
						}
					}
					break;
				case XPKINST_BIGREPEAT:
					push_repeat_marker(d, false, 16+argument, ftell(entry->xpk->file));
#ifdef LUNAPURPURA_XPK_TRACE
					LPWarn(LP_SUBSYSTEM_XPK, "BIGREPEAT %d", 16+argument);
#endif
					break;
				case XPKINST_XSKIP:
					if (argument) {
						d->cur_x += argument;
#ifdef LUNAPURPURA_XPK_TRACE
						LPWarn(LP_SUBSYSTEM_XPK, "XSKIP %d", argument);
#endif
					} else {
						ReadUint16(entry->xpk->file, 1, &d->next_holder);
						d->cur_x += d->next_holder;
#ifdef LUNAPURPURA_XPK_TRACE
						LPWarn(LP_SUBSYSTEM_XPK, "XSKIP %d", d->next_holder);
#endif
					}
					break;
				case XPKINST_BIGXSKIP:
					d->cur_x += (16 + argument);
#ifdef LUNAPURPURA_XPK_TRACE
					LPWarn(LP_SUBSYSTEM_XPK, "BIGXSKIP %d", 16+argument);
#endif
					break;
				case XPKINST_RLE:
					/* 0x40 should be safe to ignore */
					d->n_reps = argument;
#ifdef LUNAPURPURA_XPK_TRACE
					LPWarn(LP_SUBSYSTEM_XPK, "RLE %d", argument);
#endif
					break;
				case XPKINST_BIGRLE:
					d->n_reps = (16 + argument);
#ifdef LUNAPURPURA_XPK_TRACE
					LPWarn(LP_SUBSYSTEM_XPK, "BIGRLE %d", 16+argument);
#endif
					break;
				case XPKINST_DIRECT:
					{
						if (argument) {
							d->direct_counter = argument;
#ifdef LUNAPURPURA_XPK_TRACE
							LPWarn(LP_SUBSYSTEM_XPK, "DIRECT %d", argument);
#endif
						} else {
							ReadUint16(entry->xpk->file, 1, &d->next_holder);
							d->direct_counter = d->next_holder;
#ifdef LUNAPURPURA_XPK_TRACE
							LPWarn(LP_SUBSYSTEM_XPK, "DIRECT %d", d->next_holder);
#endif
						}
					}
					break;
				case XPKINST_BIGDIRECT:
					d->direct_counter = (16 + argument);
#ifdef LUNAPURPURA_XPK_TRACE
					LPWarn(LP_SUBSYSTEM_XPK, "BIGDIRECT %d", 16+argument);
#endif
					break;
				case XPKINST_LINEREPEAT:
					{
						switch (byte) {
						case XPKINST_LINEREPEAT_END:
							{
								struct RepeatMarker *marker = current_repeat_marker(d);
								if (marker->reps > 0) {
									marker->reps--;
								}
								if (marker->reps > 0) {
									fseek(entry->xpk->file, marker->loc, SEEK_SET);
								} else {
									pop_repeat_marker(d);
								}
								NEWLINE(d);
#ifdef LUNAPURPURA_XPK_TRACE
								LPWarn(LP_SUBSYSTEM_XPK, "LINEREPEAT_END (%d remaining)", marker->reps);
#endif
								break;
							}
						default:
							push_repeat_marker(d, true, argument, ftell(entry->xpk->file));
							NEWLINE(d);
#ifdef LUNAPURPURA_XPK_TRACE
							LPWarn(LP_SUBSYSTEM_XPK, "LINEREPEAT %d", argument);
#endif
						}
					}
					break;
				case XPKINST_SETXNL:
					if (argument) {
						d->cur_x = argument;
#ifdef LUNAPURPURA_XPK_TRACE
						LPWarn(LP_SUBSYSTEM_XPK, "SETXNL %d", argument);
#endif
					} else {
						ReadUint16(entry->xpk->file, 1, &d->next_holder);
						d->cur_x = d->next_holder;
#ifdef LUNAPURPURA_XPK_TRACE
						LPWarn(LP_SUBSYSTEM_XPK, "SETXNL %d", d->next_holder);
#endif
					}
					d->cur_y++;
					break;
				case XPKINST_BIGSETXNL:
					d->cur_x = (16 + argument);
					d->cur_y++;
#ifdef LUNAPURPURA_XPK_TRACE
					LPWarn(LP_SUBSYSTEM_XPK, "BIGSETXNL %d", 16+argument);
#endif
					break;
				case XPKINST_RLENL:
					d->n_reps = argument;
					NEWLINE(d);
#ifdef LUNAPURPURA_XPK_TRACE
					LPWarn(LP_SUBSYSTEM_XPK, "RLENL %d", argument);
#endif
					break;
				case XPKINST_BIGRLENL:
					d->n_reps = (16 + argument);
					NEWLINE(d);
#ifdef LUNAPURPURA_XPK_TRACE
					LPWarn(LP_SUBSYSTEM_XPK, "BIGRLENL %d", 16+argument);
#endif
					break;
				case XPKINST_DIRECTNL:
					if (argument) {
						d->direct_counter = argument;
#ifdef LUNAPURPURA_XPK_TRACE
						LPWarn(LP_SUBSYSTEM_XPK, "DIRECTNL %d", argument);
#endif
					} else {
						ReadUint16(entry->xpk->file, 1, &d->next_holder);
						d->direct_counter = d->next_holder;
#ifdef LUNAPURPURA_XPK_TRACE
						LPWarn(LP_SUBSYSTEM_XPK, "DIRECTNL %d", d->next_holder);
#endif
					}
					NEWLINE(d);
					break;
				case XPKINST_BIGDIRECTNL:
					d->direct_counter = (16 + argument);
					NEWLINE(d);
#ifdef LUNAPURPURA_XPK_TRACE
					LPWarn(LP_SUBSYSTEM_XPK, "BIGDIRECTNL %d", 16+argument);
#endif
					break;
				default:
					LPWarn(LP_SUBSYSTEM_XPK, "?? unhandled XPK instruction: 0x%X, ftell=%ld", byte, ftell(entry->xpk->file));
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

#ifdef LUNAPURPURA_PNG_SUPPORT
LPStatus
XPKDecoder_RGBAToPNG(const uint8_t *rgba, const XPKEntry *entry, const char *path)
{
	int status = stbi_write_png(path, entry->width, entry->height, 4, rgba, entry->width*4);
	if (status) {
		return LUNAPURPURA_OK;
	} else {
		LPWarn(LP_SUBSYSTEM_XPK, "stbi_write_png() failed: %d", status);
		return LUNAPURPURA_ERROR;
	}
}

#endif /* LUNAPURPURA_PNG_SUPPORT */
