FILES=src/main.py


.PHONY: flash
flash: firmware/micropython.bin
	sudo esptool.py -b 460800 write_flash -fm dio 0x00000 $^

.PHONY: upload
upload: $(FILES)
	sudo ampy -d /dev/ttyUSB0 -b 230400 put $(FILES)
	sudo ampy -d /dev/ttyUSB0 -b 230400 ls

.PHONY: run
run:
	sudo ampy -d /dev/ttyUSB0 -b 230400 run main.py


.PHONY: connect
connect:
	sudo nodemcu-uploader terminal

.PHONY: reset
reset:
	sudo esptool.py --no-stub read_mac

.PHONY: wifi
wifi:
	echo TODO: setup wifi from env WIFI PASS

