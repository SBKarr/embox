/**
 * @file pl110.c
 * @brief Initial implementation for PL110 video controller
 * @author Denis Deryugin <deryugin.denis@gmail.com>
 * @version
 * @date 09.08.2017
 */

 #include <util/log.h>

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>

#include <hal/reg.h>
#include <drivers/common/memory.h>
#include <drivers/video/fb.h>
#include <drivers/gpio.h>
#include <drivers/spi.h>

#include <framework/mod/options.h>
#include <embox/unit.h>

EMBOX_UNIT_INIT(ssd0323_init);

#define SSD0323_DISPLAY_WIDTH  OPTION_GET(NUMBER,display_width)
#define SSD0323_DISPLAY_HEIGHT OPTION_GET(NUMBER,display_height)

#define SSD0323_CS_PORT OPTION_GET(NUMBER,cs_port)
#define SSD0323_CS_PIN  OPTION_GET(NUMBER,cs_pin)
#define SSD0323_DC_PORT OPTION_GET(NUMBER,dc_port)
#define SSD0323_DC_PIN  OPTION_GET(NUMBER,dc_pin)
#define SSD0323_SPI_BUS	OPTION_GET(NUMBER,spi_bus)

static struct spi_device *ssd0323_spi_dev;

static void send_data(uint8_t *data, int len) {
	gpio_set(SSD0323_DC_PORT, 1 << SSD0323_DC_PIN, 1);
	gpio_set(SSD0323_CS_PORT, 1 << SSD0323_CS_PIN, SPI_CS_INACTIVE);
	spi_transfer(ssd0323_spi_dev, data, NULL, len);
	gpio_set(SSD0323_CS_PORT, 1 << SSD0323_CS_PIN, SPI_CS_ACTIVE);
}

static int ssd0323_set_var(struct fb_info *info,
		struct fb_var_screeninfo const *var) {
	return 0;
}

/* we use only 16 grey scale color */
static uint8_t ssd0323_fb[SSD0323_DISPLAY_WIDTH * SSD0323_DISPLAY_HEIGHT / 2]
                        __attribute__ ((aligned (0x4)));

static int ssd0323_get_var(struct fb_info *info,
		struct fb_var_screeninfo *var) {
    var->xres = SSD0323_DISPLAY_WIDTH;
    var->yres = SSD0323_DISPLAY_HEIGHT;
    var->bits_per_pixel = 4;

	return 0;
}

static void ssd0323_fillrect(struct fb_info *info, const struct fb_fillrect *rect) {
	uint32_t x, y;
	uint8_t pixel;

	pixel = (uint8_t)(rect->color & 0x0F);

	for (y = rect->dy; y < rect->dy + rect->height && y < SSD0323_DISPLAY_HEIGHT; y++) {
		for (x = rect->dx; x < rect->dx + rect->width && x < SSD0323_DISPLAY_WIDTH; x++) {
			int byte_idx = (y * SSD0323_DISPLAY_WIDTH + x) / 2;

			if (x % 2 == 0) {
				ssd0323_fb[byte_idx] = (ssd0323_fb[byte_idx] & 0x0F) | (pixel << 4);
			}
			else {
				ssd0323_fb[byte_idx] = (ssd0323_fb[byte_idx] & 0xF0) | pixel;
			}
		}
	}

	send_data(ssd0323_fb, sizeof(ssd0323_fb));
}

static const struct fb_ops ssd0323_ops = {
	.fb_set_var   = ssd0323_set_var,
	.fb_get_var   = ssd0323_get_var,
	.fb_fillrect  = ssd0323_fillrect,
};

static int ssd0323_init(void) {
	gpio_setup_mode(SSD0323_CS_PORT, 1 << SSD0323_CS_PIN, GPIO_MODE_OUT);
	gpio_setup_mode(SSD0323_DC_PORT, 1 << SSD0323_DC_PIN, GPIO_MODE_OUT);

	ssd0323_spi_dev = spi_dev_by_id(SSD0323_SPI_BUS);
	if (!ssd0323_spi_dev){
		return -ENODEV;
	}

	char *mmap_base = (void *) ssd0323_fb;

	fb_create(&ssd0323_ops, mmap_base, sizeof(ssd0323_fb));

	memset(mmap_base, 0, sizeof(ssd0323_fb));

	return 0;
}

