// Copyright (C) 2020 thisLight
// 
// luapicohttpparser is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// luapicohttpparser is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with luapicohttpparser. If not, see <http://www.gnu.org/licenses/>.

#define LUA_LIB
#include <lauxlib.h>
#include <lua.h>
#include <picohttpparser.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline int _lua_assert(lua_State *L, const char* expr, bool expr_result, const char *msg, const char *__file, int __line){
    if (!expr_result){
        luaL_error(L, "assert (%s) fail in %s (line %d): %s", expr, __file, __line, msg);
    }
}

#define lua_assert(L, expr, msg) _lua_assert(L, #expr, expr, msg, __FILE__, __LINE__)

static int lphr_parse_request(lua_State *L);

static int lphr_parse_response(lua_State *L);

// int lphr_parse_headers(lua_State *L);

// int lphr_decode_chunked(lua_State *L);

LUALIB_API int luaopen_lphr_c(lua_State *L);
