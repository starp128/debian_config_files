-- require("json")
-- result = {
--     ["ip"]="192.168.0.177",
--     ["date"]="2018-1-21",
--          }

-- local test = assert(io.open("/tmp/abc.txt", "w"))
-- result = json.encode(result)
-- test:write(result)
-- test:close()

JSON = (loadfile "JSON.lua")()

-- local lua_value = JSON:decode(raw_json_text) -- decode example

result = {
    ["ip"]="192.168.0.177",
    ["date"]="2018-1-21",
        }


local raw_json_text    = JSON:encode(result)
local pretty_json_text = JSON:encode_pretty(result)

print(raw_json_text)
print(pretty_json_text)
