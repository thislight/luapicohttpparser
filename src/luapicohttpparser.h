#define LUA_LIB
#include <lauxlib.h>
#include <lua.h>
#include <picohttpparser.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int lphr_parse_request(lua_State *L);

// int lphr_parse_response(lua_State *L);

// int lphr_parse_headers(lua_State *L);

// int lphr_decode_chunked(lua_State *L);

LUALIB_API int luaopen_lphr_c(lua_State *L);
