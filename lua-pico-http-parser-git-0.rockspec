rockspec_format = "3.0"
package = "lua-pico-http-parser"
version = "git-0"
source = {
   url = "git+https://github.com/thislight/luapicohttpparser.git",
}
description = {
   homepage = "https://github.com/thislight/luapicohttpparser",
   license = "LGPL-3"
}
build = {
   type = "builtin",
   modules = {
      lphr = "src/luapicohttpparser.lua",
      ['lphr.c'] = {
         sources = {'src/luapicohttpparser.c', 'picohttpparser/picohttpparser.c'},
         incdirs = {'src/', 'picohttpparser/'},
         libdirs = {'picohttpparser/'},
      }
   }
}
