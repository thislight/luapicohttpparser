local lphrc = require "lphr.c"
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

function lphr.attach_buffer(t)
    t._buffer = strbuffer()
    t._buffer.qconcat = strbuffermod_qconcat
end

function lphr.parse_request(chunkstring, data)
    if not data._buffer then
        lphr.attach_buffer(data)
    end
    data._buffer:append(chunkstring)
    if data.prev_pret then
        return data.prev_pret
    end
    local pret = lphrc.parse_request(data._buffer:qconcat(), data)
    if pret > 0 then
        data.prev_pret = pret
    end
    return pret
end

function lphr.parse_response(chunkstring, data)
    if not data._buffer then
        lphr.attach_buffer(data)
    end
    data._buffer:append(chunkstring)
    if data.prev_pret then
        return data.prev_pret
    end
    local pret = lphrc.parse_response(data._buffer:qconcat(), data)
    if pret > 0 then
        data.prev_pret = pret
    end
    return pret
end

function lphr.get_body(data)
    if data.prev_pret and data._buffer then
        return string.sub(data._buffer:qconcat(), data.prev_pret+1)
    else
        return nil
    end
end

return lphr
