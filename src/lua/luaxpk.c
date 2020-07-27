/*
 * Lua bindings to Luna Purpura's XPK library
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>

#include <lputil.h>

#include <clu.h>

#include <xpk.h>
#include <xpkdecoder.h>

#define XPK_TYPE_NAME "T_XPK"

static int luaxpk_NewFromFile(lua_State *L);
static int luaxpk_EntryAtIndex(lua_State *L);
static int luaxpk_Decode(lua_State *L);
static int luaxpk_DecodeTiledMode(lua_State *L);

static const luaL_Reg functions[] = {
	{"NewFromFile", luaxpk_NewFromFile},
	{"EntryAtIndex", luaxpk_EntryAtIndex},
	{"Decode", luaxpk_Decode},
	{"DecodeTiledMode", luaxpk_DecodeTiledMode},
	{NULL, NULL}
};

/* ********** */

/*
 * xpk XPK.NewFromFile(path, clu)
 */
static int
luaxpk_NewFromFile(lua_State *L)
{
	const char *path = luaL_checkstring(L, 1);
	luaL_checkudata(L, 2, "T_CLU"); /* XXX DRY plz */
	CLU **clu = lua_touserdata(L, 2);
	lua_pop(L, 2);

	XPK **xpk = lua_newuserdata(L, sizeof(xpk));
	luaL_getmetatable(L, XPK_TYPE_NAME);
	lua_setmetatable(L, -2);

	FILE *fp = fopen(path, "rb");

	if (!fp) {
		return luaL_error(L, "%s: %s", path, strerror(errno));
	}

	LPStatus status;
	*xpk = XPK_NewFromFile(fp, &status);

	if (status != LUNAPURPURA_OK) {
		return luaL_error(L, "%s", LPStatusString(status));
	}

	XPK_AttachCLU(*xpk, *clu);
	return 1;
}


/*
 * xpk_entry (table) = XPK.EntryAtIndex(xpk, index)
 *
 * Formats a table for you.
 *
 * 'index' follows the Lua convention of starting with 1, but the C API has
 * arrays beginning with 0. Therefore we subtract 1 under the hood.
 *
 * XXX this should probably return a userdata instead, which would mean
 * creating a new "T_XPKENTRY"
 */
static int
luaxpk_EntryAtIndex(lua_State *L)
{
	luaL_checkudata(L, 1, XPK_TYPE_NAME);
	XPK **xpk = lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2);
	index--; /* Acommodate for Lua's 1-based convention */
	lua_pop(L, 2);

	XPKEntry *entry = XPK_EntryAtIndex(*xpk, index);

	if (!entry) {
		return luaL_error(L, "no such XPKEntry at index %d (= Lua index %d)", index+1, index);
	}

	lua_newtable(L);
	lua_pushinteger(L, entry->width);
	lua_setfield(L, -2, "width");
	lua_pushinteger(L, entry->height);
	lua_setfield(L, -2, "height");
	lua_pushinteger(L, entry->x);
	lua_setfield(L, -2, "x");
	lua_pushinteger(L, entry->y);
	lua_setfield(L, -2, "y");


	return 1;
}


/*
 * rgba (string) = XPK.Decode(xpk, index)
 *
 * 'index' follows the Lua convention of starting with 1, but the C API has
 * arrays beginning with 0. Therefore we subtract 1 under the hood.
 */
static int
luaxpk_Decode(lua_State *L)
{
	luaL_checkudata(L, 1, XPK_TYPE_NAME);
	XPK **xpk = lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2);
	index--; /* Acommodate for Lua's 1-based convention */
	lua_pop(L, 2);

	XPKEntry *entry = XPK_EntryAtIndex(*xpk, index);

	if (!entry) {
		return luaL_error(L, "no such XPKEntry at index %d (= Lua index %d)", index+1, index);
	}

	XPKDecoder decoder;
	LPStatus status;
	uint8_t *rgba = XPKDecoder_Decode(&decoder, entry, &status);

	if (status != LUNAPURPURA_OK) {
		return luaL_error(L, "couldn't decode XPK: %s", LPStatusString(status));
	}

	lua_pushlstring(L, (const char *)rgba, entry->width*entry->height*4);

	/* pushlstring() made a copy, so let's free this */
	XPKDecoder_FreeRGBA(rgba);

	return 1;
}


/*
 * rgba (string) = XPK.DecodeTiledMode(xpk)
 */
static int
luaxpk_DecodeTiledMode(lua_State *L)
{
	luaL_checkudata(L, 1, XPK_TYPE_NAME);
	XPK **xpk = lua_touserdata(L, 1);
	lua_pop(L, 1);

	LPStatus status;
	uint8_t *rgba = XPK_DecodeTiledMode(*xpk, &status);

	if (status != LUNAPURPURA_OK) {
		return luaL_error(L, "couldn't decode XPK in tiled mode: %s", LPStatusString(status));
	}

	lua_pushlstring(L, (const char *)rgba, 640*480*4);

	/* pushlstring() made a copy, so let's free this */
	XPKDecoder_FreeRGBA(rgba);

	return 1;
}


/* ********** */

int
luaopen_luaxpk(lua_State *L)
{
	luaL_newmetatable(L, XPK_TYPE_NAME);
	luaL_register(L, "xpk", functions);
	return 1;
}
