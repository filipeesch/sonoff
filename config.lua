config = {}
config.wifi = {}

config.wifi.read = function()
    f = file.open("wifi.txt", "r");
    
    if f then
        wifiName = f:readline();
    end
end
