local lphr = require "lphr"
local lphrc = require "lphr.c"

local REPEAT_TIMES = 10^7

local dataset = {}

local CHARS = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'n', 'm', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'}

local function generate_char()
    return CHARS[math.random(1, 26)]
end

local function generate_string()
    local t = {}
    for i=1, math.random(4, 9) do
        table.insert(t, generate_char())
    end
    return table.concat(t)
end

local function generate_first_line()
    return string.format("GET /%s HTTP/1.1\r\n", generate_string())
end

local function generate_headers()
    local t = {}
    for i=1, math.random(2, 12) do
        table.insert(t, string.format("%s: %s", generate_string(), generate_string()))
    end
    return table.concat(t, "\r\n")
end

local function generate_request()
    return table.concat({
        generate_first_line(),
        generate_headers(),
        "\r\n",
        "\r\n"
    })
end

print(string.format("generating data for %d request(s)...", REPEAT_TIMES))

collectgarbage("setstepmul", 300)

for i=1, REPEAT_TIMES do
    table.insert(dataset, generate_request())
end

print("used memeory (Kbytes):", collectgarbage("count"))

collectgarbage("setstepmul", 200)

print("---- lphr.parse_request ----")
local t1 = os.time()
for _, v in ipairs(dataset) do
    if lphr.parse_request(v, {}) < 0 then
        error("lphr could not parse correctly")
    end
end
local t2 = os.time()
print(string.format("lphr.parse_request used %d sec, speed %s per second.", t2-t1, REPEAT_TIMES/(t2-t1)))


print("---- lphr.c.parse_request ----")
local t1 = os.time()
for _, v in ipairs(dataset) do
    if lphrc.parse_request(v, {}) < 0 then
        error("lphr could not parse correctly")
    end
end
local t2 = os.time()
print(string.format("lphr.c.parse_request used %d sec, speed %s request(s) per second.", t2-t1, REPEAT_TIMES/(t2-t1)))
