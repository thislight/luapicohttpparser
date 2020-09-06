local lphrc = require "lphr.c"

print("----parse_request----")

local RAW_REQUEST = {
    "GET /tamse HTTP/1.0\r\n",
    "Host: example.org\r\n",
    "\r\n",
}

local data = {}
local buffer = ""
for _, v in ipairs(RAW_REQUEST) do
    buffer = buffer..v
    local pret = lphrc.parse_request(buffer, data)
    print("pret", pret)
    if pret ~= -2 then
        break
    end
end

print("minor_version",data.minor_version)
print("method",data.method)
print("headers[1]", data.headers[1][1], data.headers[1][2])
print("path", data.path)
print("----------")

print("----parse_response----")

local RAW_RESPONSE = {
    "HTTP/1.1 200 OK\r\n",
    "Content-Length: 3059\r\n",
    "Server: GWS/2.0\r\n",
    "Connection: keep-alive\r\n",
    "\r\n",
}

local data = {}
local buffer = ""
for _, v in ipairs(RAW_RESPONSE) do
    buffer = buffer..v
    if lphrc.parse_response(buffer, data) ~= -2 then
        break
    end
end

print("minor_version",data.minor_version)
print("status", data.status)
print("message", data.message)
print("#headers", #data.headers)
print("----------")
