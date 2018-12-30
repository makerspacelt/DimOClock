FILES=src/main.py


.PHONY: flash
flash: firmware/micropython.bin
	sudo esptool.py -b 460800 write_flash -fm dio 0x00000 $^

.PHONY: upload
upload: $(FILES)
	sudo nodemcu-uploader --baud 230400 upload $(FILES)

.PHONY: run
run:
	sudo nodemcu-uploader file do Main.lua

.PHONY: connect
connect:
	sudo nodemcu-uploader terminal

.PHONY: reset
reset:
	sudo esptool.py --no-stub read_mac

.PHONY: wifi
wifi:
	echo TODO: setup wifi from env WIFI PASS

