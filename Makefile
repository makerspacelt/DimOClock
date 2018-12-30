FILES=src/main.py


.PHONY: erase
erase:
	sudo esptool.py -b 460800 erase_flash

.PHONY: flash
flash: firmware/micropython.bin
	sudo esptool.py -b 460800 write_flash -fm dio 0x00000 $^

.PHONY: upload
upload: $(FILES)

.PHONY: $(FILES)
$(FILES):
	sudo ampy -p /dev/ttyUSB0 put $@

.PHONY: ls
ls:
	sudo ampy -p /dev/ttyUSB0 ls

.PHONY: run
run:
	sudo ampy -p /dev/ttyUSB0 run main.py


.PHONY: connect
connect:
	sudo nodemcu-uploader terminal

.PHONY: reset
reset:
	sudo esptool.py --no-stub read_mac

.PHONY: wifi
wifi:
	echo TODO: setup wifi from env WIFI PASS

