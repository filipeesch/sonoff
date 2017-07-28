local config = {}
config.wifi = {}

function config.wifi.read()
    f = file.open("wifi.txt", "r")
    
    w = {}
    w.name = "name"
    w.password = "wireless"
    return w

-- if f then
--     w.name = f:readline()
--     w.password = f:readline()
--     f:close()
--     if w.name == nil or w.password == nil then
--         return nil
--     end
--     return w
-- end
-- return nil
end

config.wifi.write = function(name, password)
    f = file.open("wifi.txt", "w+")
    
    f:writeline(name)
    f:writeline(password)
    f:close()
end

return config
