-- Copyright (C) 2020 thisLight
-- 
-- luapicohttpparser is free software: you can redistribute it and/or modify
-- it under the terms of the GNU Lesser General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
-- 
-- luapicohttpparser is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU Lesser General Public License for more details.
-- 
-- You should have received a copy of the GNU Lesser General Public License
-- along with luapicohttpparser. If not, see <http://www.gnu.org/licenses/>.

local lphrc = require "lphr.c.r2"
local strbuffer = require "strbuffer"

local function strbuffermod_qconcat(self)
    if self._cache then
        local diff_new = {}
        table.move(self, self._prev_last_index+1, #self, 1, diff_new)
        self._cache = self._cache .. table.concat(diff_new, self.char)
        self._prev_last_index = #self
    else
        self._cache = table.concat(self, self.char)
        self._prev_last_index = #self
    end
    return self._cache
end

local lphr = {}

function lphr.create_buffer()
    local buffer = strbuffer()
    buffer.qconcat = strbuffermod_qconcat
    return buffer
end

function lphr.parse_request(chunkstring, buffer, last_len)
    if not buffer then
        buffer = lphr.create_buffer()
    end
    buffer:append(chunkstring)
    local pret, new_last_len, data = lphrc.parse_request(buffer:qconcat(), last_len)
    return pret, buffer, new_last_len, data
end

function lphr.parse_response(chunkstring, buffer, last_len, last_pret)
    if not buffer then
        buffer = lphr.create_buffer()
    end
    buffer:append(chunkstring)
    local pret, new_last_len, data = lphrc.parse_response(buffer:qconcat(), last_len)
    return pret, buffer, new_last_len, data
end

function lphr.get_body(buffer, pret)
    if pret > 0 then
        return string.sub(buffer:qconcat(), pret+1)
    else
        return nil
    end
end

return lphr
