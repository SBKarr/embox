#include <gen_board_conf.h>

#define GPIO0_BASE         0x40004000
#define GPIO1_BASE         0x40005000
#define GPIO2_BASE         0x40006000
#define GPIO3_BASE         0x40007000
#define GPIO4_BASE         0x40024000
#define GPIO5_BASE         0x40025000
#define GPIO6_BASE         0x40026000

#define GPIO0_IRQn         0
#define GPIO1_IRQn         1
#define GPIO2_IRQn         2
#define GPIO3_IRQn         3
#define GPIO4_IRQn         4
#define GPIO5_IRQn         30
#define GPIO6_IRQn         31

struct gpio_conf gpios[] = {
    [0] = {
        .status = ENABLED,
        .dev = {
            .name = "GPIO_PORT_A",
            .regs = {
				REGMAP("BASE", (GPIO0_BASE), 0x1000),
							},
							.irqs = {
				VAL("", GPIO0_IRQn),
							},
							.clocks = {
				VAL("", "CLK_GPIO0"),
							}
						},
        .port_num = 3,
        .port_width = 8,
    },
    [1] = {
        .status = ENABLED,
        .dev = {
            .name = "GPIO_PORT_B",
            .regs = {
                REGMAP("BASE", (GPIO1_BASE), 0x1000),
                            },
                            .irqs = {
                VAL("", GPIO1_IRQn),
                            },
                            .clocks = {
                VAL("", "CLK_GPIO1"),
                            }
                        },
        .port_num = 3,
        .port_width = 8,
    },
    [2] = {
        .status = ENABLED,
        .dev = {
            .name = "GPIO_PORT_C",
            .regs = {
                REGMAP("BASE", (GPIO2_BASE), 0x1000),
                            },
                            .irqs = {
                VAL("", GPIO2_IRQn),
                            },
                            .clocks = {
                VAL("", "CLK_GPIO2"),
                            }
                        },
        .port_num = 3,
        .port_width = 8,
    },
    [3] = {
        .status = ENABLED,
        .dev = {
            .name = "GPIO_PORT_D",
            .regs = {
                REGMAP("BASE", (GPIO3_BASE), 0x1000),
                            },
                            .irqs = {
                VAL("", GPIO3_IRQn),
                            },
                            .clocks = {
                VAL("", "CLK_GPIO3"),
                            }
                        },
        .port_num = 3,
        .port_width = 8,
    },
};

EXPORT_CONFIG(GPIO(gpios));