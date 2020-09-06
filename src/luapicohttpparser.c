#include "luapicohttpparser.h"

static const size_t LPHR_INITIAL_EXTRA_BUFFER_SIZE = 512;

static const int LPHR_DEFAULT_HEADERS_NUMBER = 64;

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


static int lphr_parse_request(lua_State *L) {
    const char* chunk = luaL_checklstring(L, 1, NULL);
    lua_Integer chunklen = luaL_len(L, 1);
    luaL_checktype(L, 2, LUA_TTABLE);
    lua_settop(L, 2);
    lua_checkstack(L, 3);
    // now stack top: #2
    lua_assert(L, chunklen <= ULONG_MAX, "expect chunk's length is not greater than the max of size_t");

    size_t last_len;
    if (lua_getfield(L, -1, "__last_len") == LUA_TNUMBER){
        last_len = (size_t)lua_tointeger(L, -1); // chunklen always is not greater than size_t
    } else {
        last_len = 0;
        lua_pushinteger(L, last_len);
        lua_setfield(L, -3, "__last_len");
    }
    lua_pop(L, 1);

    // now stack top is #2
    int minor_version;
    char *method, *path;
    size_t method_len, path_len;
    struct phr_header headers[LPHR_DEFAULT_HEADERS_NUMBER];
    size_t num_headers = sizeof(headers) / sizeof(headers[0]);
    int pret = phr_parse_request(chunk, (size_t)chunklen, (const char**)(&method), &method_len, (const char**)(&path), &path_len, &minor_version,headers, &num_headers, last_len);

    lua_pushinteger(L, chunklen);
    lua_setfield(L, -2, "__last_len");

    if (pret > 0){
        lua_pushinteger(L, minor_version);
        lua_setfield(L, -2, "minor_version");

        lua_pushlstring(L, method, method_len);
        lua_setfield(L, -2, "method");

        lua_pushlstring(L, path, path_len);
        lua_setfield(L, -2, "path");

        if (num_headers > 0) {
            if (lua_getfield(L, -1, "headers") != LUA_TTABLE) {
                lua_createtable(L, num_headers, 0);
                lua_setfield(L, -3, "headers");
            }
            lua_pop(L, 1);

            // now stack top: #2
            lua_getfield(L, -1, "headers");
            // now stack top: #2.headers (expect a table)
            for (int i = 0; i < num_headers; i++) {
                struct phr_header *header = &(headers[i]);
                lua_createtable(L, 2, 0);
                lua_pushlstring(L, header->name, header->name_len);
                lua_seti(L, -2, 1);
                lua_pushlstring(L, header->value, header->value_len);
                lua_seti(L, -2, 2);
                lua_seti(L, -2, luaL_len(L, -2) + 1);
            }
            lua_pop(L, 1);
            // now stack top: #2
        }
    }

    lua_pushinteger(L, pret);
    return 1;
}

static int lphr_parse_response(lua_State *L){
    const char* chunk = luaL_checklstring(L, 1, NULL);
    lua_Integer chunklen = luaL_len(L, 1);
    luaL_checktype(L, 2, LUA_TTABLE);
    lua_settop(L, 2);
    lua_checkstack(L, 3);
    // now stack top: #2
    lua_assert(L, chunklen <= ULONG_MAX, "expect chunk's length is not greater than the max of size_t");

    size_t last_len;
    if (lua_getfield(L, -1, "__last_len") == LUA_TNUMBER){
        last_len = (size_t)lua_tointeger(L, -1); // chunklen always is not greater than size_t
    } else {
        last_len = 0;
        lua_pushinteger(L, last_len);
        lua_setfield(L, -3, "__last_len");
    }
    lua_pop(L, 1);

    // now stack top is #2
    int minor_version, status;
    char *msg;
    size_t msglen = 0;
    struct phr_header headers[LPHR_DEFAULT_HEADERS_NUMBER];
    size_t num_headers = sizeof(headers) / sizeof(headers[0]);
    int pret = phr_parse_response(chunk, (size_t)chunklen, &minor_version, &status, (const char**)&msg, &msglen, headers, &num_headers, last_len);

    lua_pushinteger(L, chunklen);
    lua_setfield(L, -2, "__last_len");

    if (pret > 0){
        lua_pushinteger(L, minor_version);
        lua_setfield(L, -2, "minor_version");

        lua_pushinteger(L, status);
        lua_setfield(L, -2, "status");

        lua_pushlstring(L, msg, msglen);
        lua_setfield(L, -2, "message");

        if (num_headers > 0) {
            if (lua_getfield(L, -1, "headers") != LUA_TTABLE) {
                lua_createtable(L, num_headers, 0);
                lua_setfield(L, -3, "headers");
            }
            lua_pop(L, 1);

            // now stack top: #2
            lua_getfield(L, -1, "headers");
            // now stack top: #2.headers (expect a table)
            for (int i = 0; i < num_headers; i++) {
                struct phr_header *header = &(headers[i]);
                lua_createtable(L, 2, 0);
                lua_pushlstring(L, header->name, header->name_len);
                lua_seti(L, -2, 1);
                lua_pushlstring(L, header->value, header->value_len);
                lua_seti(L, -2, 2);
                lua_seti(L, -2, luaL_len(L, -2) + 1);
            }
            lua_pop(L, 1);
            // now stack top: #2
        }
    }

    lua_pushinteger(L, pret);
    return 1;
}

static const luaL_Reg lphr_c_lib[] = {
        {"parse_request", lphr_parse_request},
        {"parse_response", lphr_parse_response},
        {NULL, NULL},
    };

LUA_LIB int luaopen_lphr_c(lua_State *L){
    luaL_newlib(L, lphr_c_lib);
    return 1;
}
