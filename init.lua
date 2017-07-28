station_cfg = {}
station_cfg.ssid = "Esch"
station_cfg.pwd = "wireless"
wifi.sta.config(station_cfg)

wifi.eventmon.register(wifi.eventmon.STA_GOT_IP, function()
        
        print("IP: " .. wifi.sta.getip())
        
        srv = net.createServer(net.TCP)
        srv:listen(80, function(conn)
            conn:on("receive", function(sck, payload)
                sck:send("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1> Hello, NodeMCU.</h1>")
            end)
            conn:on("sent", function(sck)sck:close() end)
        end)
        
        m = mqtt.Client("clientid765", 120)
        
        m:on("connect", function(client)print("connected") end)
        m:on("offline", function(client)print("offline") end)
        
        m:on("message", function(client, topic, data)
            print(topic .. ":")
            if data ~= nil then
                print(data)
                client:publish("/resposta", "Recebi: '" .. data .. "' em '" .. topic .. "'", 0, 0)
            end
        end)
        
        m:connect("192.168.86.100", 1883, 0, function(client)
            print("connected")
            
            client:subscribe("/topic", 0, function(client)print("subscribe success") end)
        
        end,
        function(client, reason)
            print("failed reason: " .. reason)
        end)
        
        m:close()
end)
