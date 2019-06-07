#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "gpio.h"
#include "os_type.h"


static const char cs1_pin = 13;
static const char wr_pin = 14;
static const char data_pin = 12;

static bool blinky = true;
static char score[] = {0, 0};

static os_timer_t blinky_dot_timer;
static os_timer_t render_timer;

uint32 ICACHE_FLASH_ATTR user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }
    return rf_cal_sec;
}


void ICACHE_FLASH_ATTR blinky_callback(void *arg)
{
	blinky = (blinky)?false:true;
	if (blinky) {
		//ht1632_set_pixel_at(23, 0);
	} else {
	//	ht1632_clear_pixel_at(23, 0);
	}
}


void ICACHE_FLASH_ATTR ScoreReceived(int p1, int p2, int max)
{
	//ht1632_draw_score(p1, p2);
}



void ICACHE_FLASH_ATTR render_callback(void *arg)
{
	//ht1632_render();
}

void ICACHE_FLASH_ATTR user_init()
{
	gpio_init();

	uart_init(921600, 921600);
	os_printf("Welcome to Light-Pong Display!\n");

	//initNetwork();

	// setup ht1632 display
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
	gpio_output_set(0, 0, (1 << cs1_pin), 0);
	io_pin_set(cs1_pin, 1);

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
	gpio_output_set(0, 0, (1 << wr_pin), 0);
	io_pin_set(wr_pin, 1);

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
	gpio_output_set(0, 0, (1 << data_pin), 0);
	io_pin_set(data_pin, 1);

	//ht1632_init(cs1_pin, wr_pin, data_pin);

	os_timer_setfn(&render_timer, (os_timer_func_t *)render_callback, NULL);
	os_timer_arm(&render_timer, 50, 1);

	// ht1632_clear();
	// ht1632_draw_score(11, 13);

    //     ht1632_set_pixel_at(11, 3);
    //     ht1632_set_pixel_at(11, 4);
    //     ht1632_set_pixel_at(12, 3);
    //     ht1632_set_pixel_at(12, 4);

    //     ht1632_set_pixel_at(11, 11);
    //     ht1632_set_pixel_at(11, 12);
    //     ht1632_set_pixel_at(12, 11);
    //     ht1632_set_pixel_at(12, 12);

	os_timer_setfn(&blinky_dot_timer, (os_timer_func_t *)blinky_callback, NULL);
	os_timer_arm(&blinky_dot_timer, 500, 1);
}
