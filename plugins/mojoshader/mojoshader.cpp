#include <lua/lua.hpp>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <mojoshader/mojoshader.h>



#ifdef _WIN32
#define snprintf _snprintf   // !!! FIXME: not a safe replacement!
#endif

static const char **include_paths = NULL;
static unsigned int include_path_count = 0;

// include handling from mojoshader-compiler by Ryan C. Gordon
static int open_include(MOJOSHADER_includeType inctype, const char *fname,
                        const char *parent, const char **outdata,
                        unsigned int *outbytes, MOJOSHADER_malloc m,
                        MOJOSHADER_free f, void *d)
{
  unsigned int i;
  for (i = 0; i < include_path_count; i++)
  {
    const char *path = include_paths[i];
    const size_t len = strlen(path) + strlen(fname) + 2;
    char *buf = (char *) m(len, d);
    if (buf == NULL)
      return 0;

    snprintf(buf, len, "%s/%s", path, fname);
    FILE *io = fopen(buf, "rb");
    f(buf, d);
    if (io == NULL)
      continue;

    if (fseek(io, 0, SEEK_END) != -1)
    {
      const long fsize = ftell(io);
      if ((fsize == -1) || (fseek(io, 0, SEEK_SET) == -1))
      {
        fclose(io);
        return 0;
      } // if

      char *data = (char *) m(fsize, d);
      if (data == NULL)
      {
        fclose(io);
        return 0;
      } // if

      if (fread(data, fsize, 1, io) != 1)
      {
        f(data, d);
        fclose(io);
        return 0;
      } // if

      fclose(io);
      *outdata = data;
      *outbytes = (unsigned int) fsize;
      return 1;
    } // if
  } // for

  return 0;
} // open_include


static void close_include(const char *data, MOJOSHADER_malloc m,
                          MOJOSHADER_free f, void *d)
{
  f((void *) data, d);
} // close_include



static int mo_preprocess(lua_State *L)
{
  int stacksize = lua_gettop(L);
  int offset = 0;
  if ( lua_istable(L,-1)) {
    offset++;
  }
  if ( lua_istable(L,-2)) {
    offset++;
  }
  luaL_checkstack(L,2,"2 strings required (filename, content)");
  const char* filename = luaL_checkstring(L,-2-offset);
  size_t bufferlen;
  const char* buffer = luaL_checklstring(L,-1-offset,&bufferlen);

  std::vector<std::string>  incstrings;
  include_paths = (const char **) malloc(sizeof (char *));
  include_paths[0] = ".";
  include_path_count = 1;

  std::vector<std::string>  defstrings;
  MOJOSHADER_preprocessorDefine *defines = NULL;
  unsigned int defcount = 0;

  if (offset >= 1){
    // defines
    defcount = luaL_getn(L, -offset);
    for (unsigned int i = 1; i <= defcount; i++){
      lua_rawgeti(L, -offset, i);
      const char *ident = luaL_checkstring(L,-1);
      std::string id(ident);
      lua_pop(L,1);
      size_t pos = id.find('=');
      if (pos != std::string::npos)
      {
        std::string val = id.substr(pos+1);
        defstrings.push_back(id.erase(pos));
        defstrings.push_back(val);
      } // if
      else{
        defstrings.push_back(id);
        defstrings.push_back("");
      }
    }
    defines = (MOJOSHADER_preprocessorDefine *) malloc((defcount) * sizeof (MOJOSHADER_preprocessorDefine));
    for (unsigned int i = 0; i < defcount; i++){
      defines[i].identifier = defstrings[i*2].c_str();
      defines[i].definition = defstrings[i*2+1].c_str();
    }
  }
  else if (offset > 1){
    unsigned int inccount = luaL_getn(L, -1);
    for (unsigned int i = 1; i < inccount; i++){
      lua_rawgeti(L, -1, i);
      const char *name = luaL_checkstring(L,-1);
      incstrings.push_back(name);
      lua_pop(L,1);
    }
    include_path_count = (inccount + 1);
    include_paths = (const char**) realloc(include_paths, include_path_count * sizeof (const char*));
    for (unsigned int i = 1; i <= inccount; i++){
      include_paths[i] = incstrings[i].c_str();
    }
  }

  const MOJOSHADER_preprocessData *pd = 
    MOJOSHADER_preprocess(filename,
    buffer, bufferlen,
    defines,
    defcount,
    open_include,
    close_include,
    NULL,NULL,NULL);
  
  if (pd->error_count){
    std::stringstream errorout;
    for (int i = 0; i < pd->error_count; i++){
      char error[1024];
      snprintf(error,1024,"%s:%d - %s",pd->errors[i].filename,pd->errors[i].error_position,pd->errors[i].error);
      errorout << error << std::endl;
    }
    lua_pushstring(L,errorout.str().c_str());
    free(include_paths);
    free(defines);
    MOJOSHADER_freePreprocessData(pd);
    lua_error(L);
    return 1;
  }
  free(include_paths);
  free(defines);
  lua_pushlstring(L,pd->output,pd->output_len);
  MOJOSHADER_freePreprocessData(pd);

  return 1;
}

static const struct luaL_reg mojoshaderLib[] = {
  {"preprocess", mo_preprocess},  // string : stringfilename, string content, [{string defines}],[{string includes}]
  {NULL, NULL},
};


#define LIBAPI __declspec(dllexport)

extern "C" {
  LIBAPI int luaopen_mojoshader(lua_State *L);
};

LIBAPI int luaopen_mojoshader(lua_State *L)
{
  // openlib pushes a table on top of the lua stack
  // into which the functions were registered
  luaL_openlib (L, "mojoshader", mojoshaderLib, 0);

  // and tell lua that we returned a table on the stack
  return 1;
}


