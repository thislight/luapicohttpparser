local lphrc = require "lphr.c"

RAW_REQUEST = {
    "GET /tamse HTTP/1.0\r\n",
    "Host: example.org\r\n",
    "\r\n",
    "\r\n",
}

local data = {}
for _, v in ipairs(RAW_REQUEST) do
    local pret = lphrc.parse_request(v, data)
    print("pret", pret)
    if pret ~= -2 then
        break
    end
end

print("minor_version",data.minor_version)
print("method",data.method)
print("headers[1]", data.headers[1][1], data.headers[1][2])
print("path", data.path)
