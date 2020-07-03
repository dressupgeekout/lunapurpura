/*
 * Lua bindings to Luna Purpura's PRD library
 */

#include <lua.h>
#include <lauxlib.h>

#include <lputil.h>

#include <prd.h>

#define PRD_TYPE_NAME "T_PRD"

static int luaprd_NewFromFile(lua_State *L);

static const luaL_Reg functions[] = {
	{"NewFromFile", luaprd_NewFromFile},
	{NULL, NULL}
};

/* ********** */

/*
 * prd = PRD.NewFromFile(path)
 */
static int
luaprd_NewFromFile(lua_State *L)
{
	const char *path = luaL_checkstring(L, 1);
	lua_pop(L, 1);
	PRD **prd = lua_newuserdata(L, sizeof(PRD));
	luaL_getmetatable(L, PRD_TYPE_NAME);
	lua_setmetatable(L, -2);
	*prd = PRD_NewFromFile(path);
	return 1;
}

/* ********** */

int
luaopen_luaprd(lua_State *L)
{
	luaL_newmetatable(L, PRD_TYPE_NAME);
	luaL_register(L, "prd", functions);
	return 1;
}
