local lphrc = require "lphr.c"
local strbuffer = require "strbuffer"

local lphr = {}

function lphr.attach_buffer(t)
    t._buffer = strbuffer()
end

function lphr.parse_request(chunkstring, data)
    if not data._buffer then
        lphr.attach_buffer(data)
    end
    data._buffer:append(chunkstring)
    return lphrc.parse_request(tostring(data._buffer), data)
end

function lphr.parse_response(chunkstring, data)
    if not data._buffer then
        lphr.attach_buffer(data)
    end
    data._buffer:append(chunkstring)
    return lphrc.parse_response(tostring(data._buffer), data)
end

return lphr
