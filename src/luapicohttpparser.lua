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
    if data.prev_pret then
        return data.prev_pret
    end
    local pret = lphrc.parse_request(tostring(data._buffer), data)
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
    local pret = lphrc.parse_response(tostring(data._buffer), data)
    if pret > 0 then
        data.prev_pret = pret
    end
    return pret
end

function lphr.get_body(data)
    if data.prev_pret and data._buffer then
        return string.sub(tostring(data._buffer), data.prev_pret+1)
    else
        return nil
    end
end

return lphr
