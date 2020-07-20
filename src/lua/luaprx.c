/*
 * Lua bindings to Luna Purpura's Presage Archive library
 */

#include <lua.h>
#include <lauxlib.h>

#include <lputil.h>

#include <presagearchive.h>
#include <presagearchivemember.h>

#define PRESAGEARCHIVE_TYPE_NAME "T_PRX"

static void archive_member_to_table(lua_State *L, PresageArchive *archive, PresageArchiveMember *member);

static int luaprx_NewFromFile(lua_State *L);
static int luaprx_NEntries(lua_State *L);
static int luaprx_MemberAtIndex(lua_State *L);
static int luaprx_MemberWithResourceId(lua_State *L);

static const luaL_Reg functions[] = {
	{"NewFromFile", luaprx_NewFromFile},
	{"NEntries", luaprx_NEntries},
	{"MemberAtIndex", luaprx_MemberAtIndex},
	{"MemberWithResourceId", luaprx_MemberWithResourceId},
	{NULL, NULL}
};

/* ********** */

/*
 * INTERNAL. Creates a plain Lua table which represents the provided
 * PRXMember. Leaves the table on the top of the stack.
 */
static void
archive_member_to_table(lua_State *L, PresageArchive *archive, PresageArchiveMember *member)
{
	(void)PresageArchive_DataForMember(archive, member);

	lua_newtable(L);
	lua_pushstring(L, member->filetype);
	lua_setfield(L, -2, "filetype");
	lua_pushinteger(L, member->internal_id);
	lua_setfield(L, -2, "internal_id");
	lua_pushinteger(L, member->rid);
	lua_setfield(L, -2, "resource_id");
	lua_pushinteger(L, member->size);
	lua_setfield(L, -2, "size");
	lua_pushstring(L, member->name);
	lua_setfield(L, -2, "name");
	lua_pushlstring(L, (const char *)member->data, member->size);
	lua_setfield(L, -2, "data");
}

/* ********** */

/*
 * archive = PRX.NewFromFiles(path1, [path2])
 */
static int
luaprx_NewFromFile(lua_State *L)
{
	char *path1 = NULL;
	char *path2 = NULL;

	switch (lua_gettop(L)) {
	case 1:
		{
			path1 = (char *)luaL_checkstring(L, 1);
			lua_pop(L, 1);
		}
		break;
	case 2:
		{
			path1 = (char *)luaL_checkstring(L, 1);
			path2 = (char *)luaL_checkstring(L, 2);
			lua_pop(L, 2);
		}
		break;
	default:
		return luaL_error(L, "%s", "incorrect number of arguments");
	}

	PresageArchive **archive = lua_newuserdata(L, sizeof(PresageArchive));
	luaL_getmetatable(L, PRESAGEARCHIVE_TYPE_NAME);
	lua_setmetatable(L, -2);

	LPStatus status;
	*archive = PresageArchive_NewFromFiles(path1, path2, &status);
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
	PresageArchive **archive = luaL_checkudata(L, 1, PRESAGEARCHIVE_TYPE_NAME);
	lua_pop(L, 1);

	lua_pushinteger(L, (*archive)->n_entries);

	return 1;
}


/*
 * archivemember (table) = PRX.MemberAtIndex(prx, index)
 */
static int
luaprx_MemberAtIndex(lua_State *L)
{
	PresageArchive **archive = luaL_checkudata(L, 1, PRESAGEARCHIVE_TYPE_NAME);
	int index = luaL_checkint(L, 2);
	index--; /* Acommodate for Lua's 1-based convention */
	lua_pop(L, 2);

	PresageArchiveMember *member = (*archive)->members[index];
	archive_member_to_table(L, *archive, member);
	return 1;
}


/*
 * archivemember (table) = PRX.MemberWithResourceId(archive, filetype, rid)
 */
static int
luaprx_MemberWithResourceId(lua_State *L)
{
	PresageArchive **archive = luaL_checkudata(L, 1, PRESAGEARCHIVE_TYPE_NAME);
	const char *filetype = luaL_checkstring(L, 2);
	uint32_t rid = (uint32_t)luaL_checkint(L, 3);
	lua_pop(L, 3);

	PresageArchiveMember *member = PresageArchive_MemberWithResourceId(*archive, (char *)filetype, rid);
	archive_member_to_table(L, *archive, member);
	return 1;
}


/* ********** */

int
luaopen_luaprx(lua_State *L)
{
	luaL_newmetatable(L, PRESAGEARCHIVE_TYPE_NAME);
	luaL_register(L, "prx", functions);
	return 1;
}
