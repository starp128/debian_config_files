JSON = (loadfile "JSON.lua")()

io.input("ds.txt")
data=""
while true do
  print(data)
  line = io.read("l")
  if not line then break end
  data = data .. line
end

print(data)
print(type(data))

table=JSON:encode(data)
print(table)
print(type(table))
