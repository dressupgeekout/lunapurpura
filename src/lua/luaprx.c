/*
 * Lua bindings to Luna Purpura's PRX library
 */

#include <stdbool.h>

#include <lua.h>
#include <lauxlib.h>

#include <lputil.h>

#include <prx.h>
#include <prxmember.h>

#define PRX_TYPE_NAME "T_PRX"

static int luaprx_NewFromFile(lua_State *L);
static int luaprx_NEntries(lua_State *L);
static int luaprx_MemberAtIndex(lua_State *L);

static const luaL_Reg functions[] = {
	{"NewFromFile", luaprx_NewFromFile},
	{"NEntries", luaprx_NEntries},
	{"MemberAtIndex", luaprx_MemberAtIndex},
	{NULL, NULL}
};

/* ********** */

/*
 * prx = PRX.NewFromFile(path)
 */
static int
luaprx_NewFromFile(lua_State *L)
{
	const char *path = luaL_checkstring(L, 1);
	lua_pop(L, 1);

	PRX **prx = lua_newuserdata(L, sizeof(PRX));
	luaL_getmetatable(L, PRX_TYPE_NAME);
	lua_setmetatable(L, -2);

	LPStatus status;
	*prx = PRX_NewFromFile(path, true, &status);
	if (status != LUNAPURPURA_OK) {
		return luaL_error(L, "%s", LPStatusString(status));
	}

	return 1;
}

/*
 * n = PRX.NEntries(prx)
 */
static int
luaprx_NEntries(lua_State *L)
{
	PRX **prx = luaL_checkudata(L, 1, PRX_TYPE_NAME);
	lua_pop(L, 1);

	lua_pushinteger(L, (*prx)->n_entries);

	return 1;
}


/*
 * prxmember (table) = PRX.MemberAtIndex(prx, index)
 *
 * XXX There's gotta be a more built-in or just better way to make something
 * enumerable with next() right? As in:
 *
 *     for index, member in pairs(prx) ... end
 */
static int
luaprx_MemberAtIndex(lua_State *L)
{
	PRX **prx = luaL_checkudata(L, 1, PRX_TYPE_NAME);
	int index = luaL_checkint(L, 2);
	index--; /* Acommodate for Lua's 1-based convention */
	lua_pop(L, 2);

	PRXMember *member = (*prx)->members[index];

	lua_newtable(L);
	lua_pushstring(L, member->filetype);
	lua_setfield(L, -2, "filetype");
	lua_pushinteger(L, member->internal_id);
	lua_setfield(L, -2, "internal_id");
	lua_pushinteger(L, member->identifier);
	lua_setfield(L, -2, "identifier");
	lua_pushinteger(L, member->size);
	lua_setfield(L, -2, "size");
	lua_pushstring(L, member->name);
	lua_setfield(L, -2, "name");
	lua_pushlstring(L, member->data, member->size);
	lua_setfield(L, -2, "data");

	return 1;
}

/* ********** */

int
luaopen_luaprx(lua_State *L)
{
	luaL_newmetatable(L, PRX_TYPE_NAME);
	luaL_register(L, "prx", functions);
	return 1;
}
