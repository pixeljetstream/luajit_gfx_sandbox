
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

#include <string.h>

static int touserdata(lua_State *L)
{
  int stacksize = lua_gettop(L);
  int size;
  void* orig;
  void* udata;

  if (stacksize < 3){
    lua_pushstring(L,"1 lightuser data, 1 number, 1 table required");
    lua_error(L);
    return 1;
  }

  orig = lua_touserdata(L,-3);
  size = (int)luaL_checknumber(L,-2);

  if (!orig || !size || !lua_istable(L,-1)){
    lua_pushstring(L,"1 lightuser data, 1 number, 1 table required");
    lua_error(L);
    return 1;
  }
  // stack: light, number, table

  udata = lua_newuserdata(L,size);
  memcpy(udata,orig,size);
  // stack: light, number, table, udata

  lua_pushvalue(L,-2);
  // stack: light, number, table, udata, table

  lua_setmetatable(L,-2);
  // stack: light, number, table, udata

  return 1;
}

static const struct luaL_reg ffihelperLib[] = {
  {"touserdata", touserdata},
  {NULL, NULL},
};


#define LIBAPI __declspec(dllexport)

LIBAPI int luaopen_ffihelper(lua_State *L)
{
  // openlib pushes a table on top of the lua stack
  // into which the functions were registered
  luaL_openlib (L, "ffihelper", ffihelperLib, 0);

  // and tell lua that we returned a table on the stack
  return 1;
}

