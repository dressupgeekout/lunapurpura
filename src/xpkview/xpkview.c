/*
 * xpkview
 *
 * This file is part of Luna Purpura.
 */

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include <lputil.h>
#include <xpk.h>
#include <xpkdecoder.h>

/* ********** */

#define MSGPREFIX "xpkview"
#define WINDOW_TITLE "Luna Purpura: XPK Viewer"

struct XPKViewState {
	SDL_Window *window;
	SDL_Renderer *renderer;

	int which_entry;
	char *xpk_path;
	FILE *xpk_fp;
	XPK *xpk;
	XPKEntry *xpkentry;
	SDL_Texture *xpkentry_tx;
	SDL_Rect entry_rect;

	uint8_t *checkerboard_rgba;
	SDL_Texture *checkerboard_tx;
	bool centered;
	bool checkerboard_on;
};
static struct XPKViewState app;

static bool XPKView_Setup(void);
static bool XPKView_CreateCheckerboardPattern(void);
static bool XPKView_MainLoop(void);
static void XPKView_Shutdown(void);
static bool XPKView_HandleKeyboard(SDL_Keycode key);
static bool XPKView_LoadXPKEntry(void);
static void XPKView_NextEntry(void);
static void XPKView_PreviousEntry(void);
static void XPKView_ToggleCentered(void);
static void XPKView_ToggleCheckerboard(void);

/* ********** */

int
main(int argc, char *argv[])
{
	app.which_entry = 0;
	app.xpk_path = NULL;
	app.xpk_fp = NULL;
	app.checkerboard_rgba = NULL;
	app.xpk = NULL;
	app.xpkentry = NULL;
	app.xpkentry_tx = NULL;
	app.entry_rect = (SDL_Rect){0, 0, 0, 0};
	app.centered = false;
	app.checkerboard_on = true;

	if (argc < 3) {
		LPWarn(MSGPREFIX, "usage: xpkview clufile xpkfile");
		goto fail;
	}

	char *clu_path = argv[1];
	app.xpk_path = argv[2];

	FILE *clu_fp = fopen(clu_path, "rb");
	if (!clu_fp) {
		LPWarn(MSGPREFIX, "couldn't open CLU file: %s: %s", clu_path, strerror(errno));
		goto fail;
	}

	LPStatus status;

	CLU *clu = CLU_NewFromFile(clu_fp, &status);
	if (status != LUNAPURPURA_OK) {
		LPWarn(MSGPREFIX, "couldn't open CLU file: %s: %s", clu_path, strerror(errno));
		goto fail;
	}

	app.xpk_fp = fopen(app.xpk_path, "rb");
	if (!app.xpk_fp) {
		LPWarn(MSGPREFIX, "couldn't open XPK file: %s: %s", app.xpk_path, strerror(errno));
		goto fail;
	}

	app.xpk = XPK_NewFromFile(app.xpk_fp, &status);
	if (status != LUNAPURPURA_OK) {
		LPWarn(MSGPREFIX, "couldn't open XPK file: %s: %s", app.xpk_path, LPStatusString(status));
		goto fail;
	}

	XPK_AttachCLU(app.xpk, clu);

	if (!XPKView_Setup()) {
		goto fail;
	}

	if (!XPKView_CreateCheckerboardPattern()) {
		goto fail;
	}

	XPKView_LoadXPKEntry();

	/* Obtain the checkerboard texture. */
	SDL_Surface *checkerboard_surf = SDL_CreateRGBSurfaceWithFormatFrom(
		app.checkerboard_rgba, 32, 32, 32, 32*4, SDL_PIXELFORMAT_RGBA32
	);
	free(app.checkerboard_rgba);
	app.checkerboard_tx = SDL_CreateTextureFromSurface(app.renderer, checkerboard_surf);
	SDL_FreeSurface(checkerboard_surf);

	if (!XPKView_MainLoop()) {
		goto fail;
	}

	XPKView_Shutdown();
	return EXIT_SUCCESS;

fail:
	XPKView_Shutdown();
	return EXIT_FAILURE;
}


static bool
XPKView_Setup(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		LPWarn(MSGPREFIX, "Couldn't initialize SDL: %s", SDL_GetError());
		return false;
	}

	if (SDL_CreateWindowAndRenderer(640, 480, 0, &app.window, &app.renderer) != 0) {
		LPWarn(MSGPREFIX, "Couldn't create window and/or renderer: %s", SDL_GetError());
		return false;
	}

	SDL_SetWindowTitle(app.window, WINDOW_TITLE);
	SDL_ShowWindow(app.window);
	return true;
}


/*
 * The checkerboard pattern consists of alternating white and gray squares.
 * Each square is 16x16 pixels. But we only need to allocate a 2x2 version of
 * this pattern (white, gray, NEWLINE, gray, white).
 */
static bool
XPKView_CreateCheckerboardPattern(void)
{
	int checker_w = 16;
	int checker_h = 16;
	SDL_Color white = (SDL_Color){255, 255, 255, 255};
	SDL_Color gray = (SDL_Color){240, 240, 240, 240};

	app.checkerboard_rgba = calloc(4, checker_w*checker_h*4);

	if (!app.checkerboard_rgba) {
		LPWarn(MSGPREFIX, "couldn't malloc the checkerboard!");
		return false;
	}

	int i = 0;

	/* Top row: WHITE, GRAY */
	for (int y = 0; y < checker_h; y++) {
		for (int x = 0; x < checker_w; x++) {
			app.checkerboard_rgba[i++] = white.r;
			app.checkerboard_rgba[i++] = white.g;
			app.checkerboard_rgba[i++] = white.b;
			app.checkerboard_rgba[i++] = white.a;
		}

		for (int x = 0; x < checker_w; x++) {
			app.checkerboard_rgba[i++] = gray.r;
			app.checkerboard_rgba[i++] = gray.g;
			app.checkerboard_rgba[i++] = gray.b;
			app.checkerboard_rgba[i++] = gray.a;
		}
	}

	/* Bottom row: GRAY, WHITE */
	for (int y = 0; y < checker_h; y++) {
		for (int x = 0; x < checker_w; x++) {
			app.checkerboard_rgba[i++] = gray.r;
			app.checkerboard_rgba[i++] = gray.g;
			app.checkerboard_rgba[i++] = gray.b;
			app.checkerboard_rgba[i++] = gray.a;
		}

		for (int x = 0; x < checker_w; x++) {
			app.checkerboard_rgba[i++] = white.r;
			app.checkerboard_rgba[i++] = white.g;
			app.checkerboard_rgba[i++] = white.b;
			app.checkerboard_rgba[i++] = white.a;
		}
	}

	return true;
}


static bool
XPKView_MainLoop(void)
{
	bool done = false;
	bool want_quit = false;
	SDL_Event event;

	int win_w, win_h;
	SDL_GetWindowSize(app.window, &win_w, &win_h);

	while (!done) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				want_quit = XPKView_HandleKeyboard(event.key.keysym.sym);
				break;
			case SDL_QUIT:
				want_quit = true;
				break;
			default:
				; /* OK */
			}
		}

		if (want_quit) {
			done = true;
		}

		/* Now we can draw stuff. */
		SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);
		SDL_RenderClear(app.renderer);

		if (app.checkerboard_on) {
			for (int i = 0; i < win_h/32; i++) {
				for (int j = 0; j < win_w/32; j++) {
					SDL_Rect dstrect = (SDL_Rect){32*j, 32*i, 32, 32};
					SDL_RenderCopy(app.renderer, app.checkerboard_tx, NULL, &dstrect);
				}
			}
		}

		SDL_RenderCopy(app.renderer, app.xpkentry_tx, NULL, &app.entry_rect);

		SDL_RenderPresent(app.renderer);

		/* LOVE2D does this. It can dramatically reduce CPU usage. */
		SDL_Delay(1);
	}

	return true;
}


static void
XPKView_Shutdown(void)
{
	if (app.window) SDL_DestroyWindow(app.window);
	if (app.renderer) SDL_DestroyRenderer(app.renderer);
	if (app.checkerboard_tx) SDL_DestroyTexture(app.checkerboard_tx);
	if (app.xpkentry_tx) SDL_DestroyTexture(app.xpkentry_tx);
	if (app.xpk) XPK_Free(app.xpk);
	SDL_Quit();
}


/*
 * The return value indicates whether we want to quit now, or not.
 */
static bool
XPKView_HandleKeyboard(SDL_Keycode key)
{
	switch (key) {
	case SDLK_LEFT:
		XPKView_PreviousEntry();
		break;
	case SDLK_RIGHT:
		XPKView_NextEntry();
		break;
	case SDLK_b:
		XPKView_ToggleCheckerboard();
		break;
	case SDLK_c:
		XPKView_ToggleCentered();
		break;
	case SDLK_q:
		return true;
		break;
	default:
		; /* OK */
	}

	return false;
}


/*
 * Loading a new XPKEntry implies destroying the currently active one.
 */
static bool
XPKView_LoadXPKEntry(void)
{
	if (app.xpkentry_tx) SDL_DestroyTexture(app.xpkentry_tx);

	app.xpkentry = XPK_EntryAtIndex(app.xpk, app.which_entry);

	LPStatus status;
	uint8_t *rgba = XPKEntry_Decode(app.xpkentry, &status);

	if (status != LUNAPURPURA_OK) {
		LPWarn(MSGPREFIX, "Couldn't decode XPK entry #%d! - %s", app.which_entry, LPStatusString(status));
		return false;
	}

	SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormatFrom(
		rgba, app.xpkentry->width, app.xpkentry->height, 32, app.xpkentry->width*4, SDL_PIXELFORMAT_RGBA32
	);
	app.xpkentry_tx = SDL_CreateTextureFromSurface(app.renderer, surf);
	SDL_FreeSurface(surf);
	XPKDecoder_FreeRGBA(rgba);

	app.entry_rect.x = app.xpkentry->x;
	app.entry_rect.y = app.xpkentry->y;
	app.entry_rect.w = app.xpkentry->width;
	app.entry_rect.h = app.xpkentry->height;

	/* Do this twice on purpose so that we preserve the current setting. */
	XPKView_ToggleCentered();
	XPKView_ToggleCentered();

	return true;
}


static void
XPKView_NextEntry(void)
{
	if (app.which_entry < (app.xpk->n_entries-1)) {
		app.which_entry++;
		XPKView_LoadXPKEntry();
	}
}


static void
XPKView_PreviousEntry(void)
{
	if (app.which_entry > 0) {
		app.which_entry--;
		XPKView_LoadXPKEntry();
	}
}


static void
XPKView_ToggleCentered(void)
{
	app.centered = !app.centered;

	int win_w, win_h;
	SDL_GetWindowSize(app.window, &win_w, &win_h);

	if (app.centered) {
		app.entry_rect.x = (win_w/2) - (app.xpkentry->width/2);
		app.entry_rect.y = (win_h/2) - (app.xpkentry->height/2);
	} else {
		app.entry_rect.x = app.xpkentry->x;
		app.entry_rect.y = app.xpkentry->y;
	}

	app.entry_rect.w = app.xpkentry->width;
	app.entry_rect.h = app.xpkentry->height;
}


static void
XPKView_ToggleCheckerboard(void)
{
	app.checkerboard_on = !app.checkerboard_on;
}
