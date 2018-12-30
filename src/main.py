import time

def do_connect():
    import network
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print('connecting to network...')
        wlan.connect('wifi-name', 'wifi-pass')
        while not wlan.isconnected():
            pass
    print('network config:', wlan.ifconfig())


def main():
    do_connect()
    while True:
        time.sleep(1)
        print('ping')


main()

