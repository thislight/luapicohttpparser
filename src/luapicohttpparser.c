#include "luapicohttpparser.h"

static const size_t LPHR_INITIAL_EXTRA_BUFFER_SIZE = 512;

static int auto_free_buffer(lua_State* L){
    void** bufferpointer = luaL_checkudata(L, 1, NULL);
    free(*bufferpointer);
    *bufferpointer = NULL;
    return 0;
}

static void set_auto_free_buffer_metatable(lua_State* L){
    lua_checkstack(L, 2);
    lua_newtable(L);
    lua_pushcfunction(L, &auto_free_buffer);
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -1);
}

/*
local lphr = require "lphr"

local data = {}
for str in source do
    lphr.parse_request(str, data)
end
*/
static int lphr_parse_request(lua_State *L) {
    const char *chunk = luaL_checklstring(L, 1, NULL);
    lua_Integer chunk_length = luaL_len(L, 1);
    luaL_checktype(L, 2, LUA_TTABLE);
    lua_settop(L, 2); // make #2 as stack top
    luaL_checkstack(L, 3, NULL);

    int type = lua_getfield(L, -1, "buffer"); // stack top: #2.buffer
    if (type == LUA_TNIL){
        char **bufferpointer = lua_newuserdata(L, sizeof(char*)); // stack top: userdata
        size_t buflen = chunk_length + LPHR_INITIAL_EXTRA_BUFFER_SIZE;
        char *buffer = malloc(buflen);
        *bufferpointer = buffer;
        set_auto_free_buffer_metatable(L);
        lua_setfield(L, -3, "buffer"); // expect stack -3: #2
        lua_pushinteger(L, buflen);
        lua_setfield(L, -3, "buffer_length"); // expect stack -3: #2
    }
    // now stack top is #2.buffer
    lua_pop(L, 1); // remove #2.buffer

    char **bufferpointer;
    if (lua_getfield(L, -1, "buffer") == LUA_TUSERDATA){
        bufferpointer = lua_touserdata(L, -1);
    } else {
        luaL_error(L, "the type of 'buffer' in #2 parameter must be 'userdata' (got %s)", luaL_typename(L, -1));
    }
    lua_pop(L, 1); // remove #2.buffer

    // now stack top is #2
    size_t buffer_length;
    if (lua_getfield(L, -1, "buffer_length") == LUA_TNUMBER){
        lua_Integer buffer_length_lua = lua_tointeger(L, -1);
        buffer_length = (size_t)buffer_length_lua;
    } else {
        luaL_error(L, "the type of 'buffer_length' in #2 parameter must be 'number' (got %s)", luaL_typename(L, -1));
    }
    lua_pop(L, 1); // remove #2.buffer_length

    // now stack top is #2
    size_t prev_buffer_length = buffer_length;
    if (chunk_length + buffer_length > prev_buffer_length){
        buffer_length += chunk_length;
        *bufferpointer = realloc(*bufferpointer, buffer_length);
        
        lua_pushinteger(L, (lua_Integer)buffer_length);
        lua_setfield(L, -2, "buffer_length"); // sync new buffer length with arguemnt #2
    }
    char *buffer_last_empty_start = *bufferpointer + prev_buffer_length;
    memcpy(buffer_last_empty_start, chunk, chunk_length);

    // now stack top is #2
    int minor_version;
    char *method, *path;
    struct phr_header headers[32];
    size_t method_len, path_len, num_headers;
    num_headers = sizeof(headers) / sizeof(headers[0]);

    int pret = phr_parse_request(*bufferpointer, buffer_length, (const char**)&method, &method_len, (const char**)(&path), &path_len, &minor_version, headers, &num_headers, prev_buffer_length);

    if (method_len){
        lua_pushlstring(L, method, method_len);
        lua_setfield(L, -2, "method");
        lua_pushinteger(L, minor_version); // when method have got, the minor version must be got
        lua_setfield(L, -2, "minor_version");
    }

    if (path_len){
        lua_pushlstring(L, path, path_len);
        lua_setfield(L, -2, "path");
    }

    if (num_headers > 0){
        if (lua_getfield(L, -1, "headers") != LUA_TTABLE){
            lua_createtable(L, num_headers, 0);
            lua_setfield(L, -3, "headers");
        }
        lua_pop(L, 1);
        // now stack top: #2
        for (int i=0; i<num_headers; i++){
            struct phr_header *header = &(headers[i]);
            lua_createtable(L, 2, 0);
            lua_pushlstring(L, header->name, header->name_len);
            lua_seti(L, -2, 1);
            lua_pushlstring(L, header->value, header->value_len);
            lua_seti(L, -2, 1);
        }
    }

    lua_pushinteger(L, pret);
    return 1;
}

static const luaL_Reg lphr_c_lib[] = {
        {"parse_request", lphr_parse_request},
        {NULL, NULL},
    };

LUA_LIB int luaopen_lphr_c(lua_State *L){
    luaL_newlib(L, lphr_c_lib);
    return 1;
}
