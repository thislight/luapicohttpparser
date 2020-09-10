local lphr = require "lphr.r2"

print("----parse_request----")

local RAW_REQUEST = {
    "GET /tamse HTTP/1.0\r\n",
    "Host: example.org\r\n",
    "\r\n",
    "Happy!\r\n"
}

local pret, buffer, last_len, data
for _, v in ipairs(RAW_REQUEST) do
    pret, buffer, last_len, data = lphr.parse_request(v, buffer, last_len)
end

print("minor_version",data.minor_version)
print("method",data.method)
print("headers[1]", data.headers[1][1], data.headers[1][2])
print("path", data.path)
print("body", lphr.get_body(buffer, pret))
print("----------")

print("----parse_response----")

local RAW_RESPONSE = {
    "HTTP/1.1 200 OK\r\n",
    "Content-Length: 3059\r\n",
    "Server: GWS/2.0\r\n",
    "Connection: keep-alive\r\n",
    "\r\n",
    "Happy!\r\n",
}

local pret, buffer, last_len, data
for _, v in ipairs(RAW_RESPONSE) do
    pret, buffer, last_len, data = lphr.parse_response(v, buffer ,last_len)
end

print("minor_version",data.minor_version)
print("status", data.status)
print("message", data.message)
print("#headers", #data.headers)
print("body", lphr.get_body(buffer, pret))
print("----------")
