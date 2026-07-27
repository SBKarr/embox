/**
 * @file
 * @brief
 *
 * @author  Perevalov Efim
 * @date
 */

#include <stdint.h>

#include <drivers/gpio.h>
#include <hal/reg.h>
#include <kernel/irq.h>

#include <framework/mod/options.h>
#include <config/board_config.h>

#define GPIO_CHIP_ID     OPTION_GET(NUMBER, gpio_chip_id)
#define GPIO_PINS_NUMBER 8

#define GPIOA ((volatile struct gpio_regs *)CONF_GPIO_PORT_A_REGION_BASE)
#define GPIOB ((volatile struct gpio_regs *)CONF_GPIO_PORT_B_REGION_BASE)
#define GPIOC ((volatile struct gpio_regs *)CONF_GPIO_PORT_C_REGION_BASE)
#define GPIOD ((volatile struct gpio_regs *)CONF_GPIO_PORT_D_REGION_BASE)

// Clocking not implemented

struct gpio_regs {
    volatile uint32_t DR;             	/*!< Data                                   Address offset: 0x000 */
	uint32_t RESERVED0[255];			/*!< Reserved                               Address offset: 0x004 */
    volatile uint32_t DDR;            	/*!< Direction                              Address offset: 0x400 */
	volatile uint32_t IS;             	/*!< Interrupt sense                        Address offset: 0x404 */
    volatile uint32_t IBE;            	/*!< Interrupt both edges                   Address offset: 0x408 */
    volatile uint32_t IEV;            	/*!< Interrupt event                        Address offset: 0x40C */
    volatile uint32_t IM;             	/*!< Interrupt mask                         Address offset: 0x410 */
    volatile uint32_t RIS;            	/*!< Raw interrupt status                   Address offset: 0x414 */
    volatile uint32_t MIS;            	/*!< Masked interrupt status                Address offset: 0x418 */
    volatile uint32_t ICR;            	/*!< Interrupt clear                        Address offset: 0x41C */
    volatile uint32_t AFSEL;          	/*!< Alternate function select              Address offset: 0x420 */
    uint32_t RESERVED1[55];				/*!< Reserved                               Address offset: 0x424 */
    volatile uint32_t DR2R;           	/*!< 2mA drive select                       Address offset: 0x500 */
    volatile uint32_t DR4R;           	/*!< 4mA drive select                       Address offset: 0x504 */
    volatile uint32_t DR8R;           	/*!< 8mA drive select                       Address offset: 0x508 */
    volatile uint32_t ODR;            	/*!< Open drain select                      Address offset: 0x50C */
    volatile uint32_t PUR;            	/*!< Pull-up select                         Address offset: 0x510 */
    volatile uint32_t PDR;            	/*!< Pull-down select                       Address offset: 0x514 */
    volatile uint32_t SLR;            	/*!< Slew rate control                      Address offset: 0x518 */
    volatile uint32_t DEN;            	/*!< Digital enable                         Address offset: 0x51C */
    volatile uint32_t LOCK;           	/*!< Lock                                   Address offset: 0x520 */
    volatile uint32_t CR;             	/*!< Commit                                 Address offset: 0x524 */
};

static const struct gpio_chip stellaris_gpio_chip;

static inline volatile struct gpio_regs *stellaris_gpio_get_gpio_port(int port) {
	switch (port) {
	case 0:
		return GPIOA;
	case 1:
		return GPIOB;
	case 2:
		return GPIOC;
		break;
	case 3:
		return GPIOD;
		break;
	default:
		return NULL;
	}

	return 0;
}

static int stellaris_gpio_setup_mode(unsigned int port, gpio_mask_t pins, uint32_t mode) {
	volatile struct gpio_regs *gpio_regs;
	// char *clk_name;

	gpio_regs = stellaris_gpio_get_gpio_port(port);
	if (gpio_regs == NULL) {
		return -1;
	}

	// switch (port) {
	// case 0:
	// 	clk_name = CONF_GPIO_PORT_A_CLK_ENABLE();
	// 	break;
	// case 1:
	// 	clk_name = CONF_GPIO_PORT_B_CLK_ENABLE();
	// 	break;
	// case 2:
	// 	clk_name = CONF_GPIO_PORT_C_CLK_ENABLE();
	// 	break;
	// default:
	// 	return -1;
	// }

	/* Enable port */
	// clk_enable(clk_name);

	if (mode & (GPIO_MODE_IN | GPIO_MODE_OUT)) {
		gpio_regs->AFSEL &= ~pins;
		gpio_regs->DEN |= pins;
	}

	if (mode & GPIO_MODE_IN) {
		gpio_regs->DDR &= ~pins;
	}

	if (mode & GPIO_MODE_OUT) {
		gpio_regs->DDR |= pins;
	}

	if (mode & GPIO_MODE_INT_SECTION) {
		// stellaris_gpio_setup_irq(port, pins, mode);
	}

	if (mode & GPIO_MODE_ALT_SECTION) {
		gpio_regs->AFSEL |= pins;
		gpio_regs->DEN |= pins;
	}

	return 0;
}

static void stellaris_gpio_set(unsigned int port, gpio_mask_t pins, int level) {
	volatile struct gpio_regs *gpio_regs;

	gpio_regs = stellaris_gpio_get_gpio_port(port);
	if (gpio_regs == NULL) {
		return;
	}

	if (level) {
		gpio_regs->DR |= pins;
	}
	else {
		gpio_regs->DR &= ~pins;
	}
}

static gpio_mask_t stellaris_gpio_get(unsigned int port, gpio_mask_t pins) {
	volatile struct gpio_regs *gpio_regs;

	gpio_regs = stellaris_gpio_get_gpio_port(port);
	if (gpio_regs == NULL) {
		return -1;
	}

	return gpio_regs->DR & pins;
}

static const struct gpio_chip stellaris_gpio_chip = {
    .setup_mode = stellaris_gpio_setup_mode,
    .get = stellaris_gpio_get,
    .set = stellaris_gpio_set,
    .nports = 3,
    .chip_id = GPIO_CHIP_ID,
};

GPIO_CHIP_DEF(&stellaris_gpio_chip);

