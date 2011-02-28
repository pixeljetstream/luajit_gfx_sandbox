
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

#include <string.h>

static int newuserdata(lua_State *L)
{
  int stacksize = lua_gettop(L);
  size_t size;

  if (stacksize < 2){
    lua_pushstring(L,"1 number, 1 table required");
    lua_error(L);
    return 1;
  }

  size = (size_t)luaL_checkinteger(L,-2);

  if (!size || !lua_istable(L,-1)){
    lua_pushstring(L,"1 number, 1 table required");
    lua_error(L);
    return 1;
  }
  // stack: number, table
  
  lua_newuserdata(L,size);
  // stack: number, table, udata

  lua_pushvalue(L,-2);
  // stack: number, table, udata, table

  lua_setmetatable(L,-2);
  // stack: number, table, udata

  return 1;
}

static const struct luaL_reg ffihelperLib[] = {
  {"newuserdata", newuserdata},
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

