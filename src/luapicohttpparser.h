#define LUA_LIB
#include <lauxlib.h>
#include <lua.h>
#include <picohttpparser.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lphr_parse_request(lua_State *L);

int lphr_parse_response(lua_State *L);

int lphr_parse_headers(lua_State *L);

int lphr_decode_chunked(lua_State *L);

LUALIB_API int luaopenlib_lphr(lua_State *L);
