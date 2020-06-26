/* 
 * Lua bindings to Luna Purpura's CLU library
 */

#include <lua.h>
#include <lauxlib.h>

#include <lpstatus.h>

#include <clu.h>

#define CLU_TYPE_NAME "T_CLU" 

static int luaclu_NewFromFile(lua_State *L);
static int luaclu_ColorAtIndex(lua_State *L);
static int luaclu_GetName(lua_State *L);

static const luaL_Reg functions[] = {
	{"NewFromFile", luaclu_NewFromFile},
	{"ColorAtIndex", luaclu_ColorAtIndex},
	{"GetName", luaclu_GetName},
	{NULL, NULL}
};

/* ********** */

/*
 * clu = CLU.NewFromFile(path)
 */
static int
luaclu_NewFromFile(lua_State *L)
{
	const char *path = luaL_checkstring(L, 1);
	lua_pop(L, 1);
	CLU **clu = lua_newuserdata(L, sizeof(CLU));
	luaL_getmetatable(L, CLU_TYPE_NAME);
	lua_setmetatable(L, -2);
	LPStatus status;
	*clu = CLU_NewFromFile(path, &status); /* XXX check status */
	return 1;
}


/*
 * r, g, b = CLU.ColorAtIndex(clu, index)
 *
 * 'index' follows the Lua convention of starting with 1, but the C API has
 * arrays beginning with 0. Therefore we subtract 1 under the hood.
 */
static int
luaclu_ColorAtIndex(lua_State *L)
{
	luaL_checkudata(L, 1, CLU_TYPE_NAME);
	CLU **clu = lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2);
	index--; /* Acommodate for Lua's 1-based convention */
	lua_pop(L, 2);
	const uint8_t *color = CLU_ColorAtIndex(*clu, index);
	lua_pushinteger(L, color[0]);
	lua_pushinteger(L, color[1]);
	lua_pushinteger(L, color[2]);
	return 3;
}


/*
 * name = CLU.GetName(clu)
 */
static int
luaclu_GetName(lua_State *L)
{
	luaL_checkudata(L, 1, CLU_TYPE_NAME);
	CLU **clu = lua_touserdata(L, 1);
	lua_pop(L, 1);
	lua_pushstring(L, (*clu)->name);
	return 1;
}

/* ********** */

int
luaopen_luaclu(lua_State *L)
{
	luaL_newmetatable(L, CLU_TYPE_NAME);
	luaL_register(L, "clu", functions);
	return 1;
}
