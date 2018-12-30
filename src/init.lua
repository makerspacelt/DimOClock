

-- setup

if adc.force_init_mode(adc.INIT_ADC)
then
	node.restart();
	return;
end

wifi.setmode(wifi.STATION);
wifi.sta.sethostname("dim-o-clock");
--wifi.sta.config("wifi-name", "wifi-pass");


CS_PIN = 2;
WR_PIN = 3;
DATA_PIN = 4;

gpio.mode(CS_PIN, gpio.OUTPUT);
gpio.mode(WR_PIN, gpio.OUTPUT);
gpio.mode(DATA_PIN, gpio.OUTPUT);


sntp.sync("lt.pool.ntp.org",
	function(sec, usec, server, info)
		print('sntp synced', sec, usec, server)
	end,
	function(err, msg)
		print('sntp sync failed!', err, msg)
	end,
	1
)

tmr.alarm(0, 1000, 1, function ()
	time = rtctime.epoch2cal(rtctime.get());
	darkness=adc.read(0);
	print("-----------------");
	print("darkness:",darkness);
	print("IP:", wifi.sta.getip(), "wifi:", wifi.sta.status());
	print(string.format("time: %02d:%02d:%02d", time["hour"], time["min"], time["sec"]));

end)

