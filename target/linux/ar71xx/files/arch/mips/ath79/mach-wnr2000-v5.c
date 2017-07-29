/*
 *  NETGEAR WNR2000v5 board support
 *
 * Copyright (c) 2017 Raphael Catolino <raphael.catolino@gmail.com>
 * Copyright (c) 2013-2015 The Linux Foundation. All rights reserved.
 * Copyright (c) 2012 Gabor Juhos <juhosg@openwrt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"

#define WNR2000V5_GPIO_LED_WLAN		12
#define WNR2000V5_GPIO_LED_STATUS		13

#define WNR2000V5_GPIO_LED_WAN		4
#define WNR2000V5_GPIO_LED_LAN1		16
#define WNR2000V5_GPIO_LED_LAN2		15
#define WNR2000V5_GPIO_LED_LAN3		14
#define WNR2000V5_GPIO_LED_LAN4		11

#define WNR2000V5_GPIO_BTN_RFKILL 0
#define WNR2000V5_GPIO_BTN_RESET  1
#define WNR2000V5_GPIO_BTN_WPS    2

#define WNR2000V5_KEYS_POLL_INTERVAL	20	/* msecs */
#define WNR2000V5_KEYS_DEBOUNCE_INTERVAL	(4 * WNR2000V5_KEYS_POLL_INTERVAL)

static struct gpio_led wnr2000v5_leds_gpio[] __initdata = {
	{
		.name		= "wnr2000v5:green:status",
		.gpio		= WNR2000V5_GPIO_LED_STATUS,
		.active_low	= 1,
	},
	{
		.name		= "wnr2000v5:green:wlan",
		.gpio		= WNR2000V5_GPIO_LED_WLAN,
		.active_low	= 1,
	}
};

static struct gpio_keys_button wnr2000v5_gpio_keys[] __initdata = {
	{
		.desc		= "WPS button",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = WNR2000V5_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= WNR2000V5_GPIO_BTN_WPS,
		.active_low	= 1,
	},
	{
		.desc		= "rfkill",
		.type		= EV_KEY,
		.code		= KEY_RFKILL,
		.debounce_interval = WNR2000V5_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= WNR2000V5_GPIO_BTN_RFKILL,
		.active_low	= 1,
	},
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = WNR2000V5_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= WNR2000V5_GPIO_BTN_RESET,
		.active_low	= 1,
	},
};

static void __init wnr2000v5_gpio_led_setup(void)
{
	ath79_gpio_direction_select(WNR2000V5_GPIO_LED_WAN, true);
	ath79_gpio_direction_select(WNR2000V5_GPIO_LED_LAN1, true);
	ath79_gpio_direction_select(WNR2000V5_GPIO_LED_LAN2, true);
	ath79_gpio_direction_select(WNR2000V5_GPIO_LED_LAN3, true);
	ath79_gpio_direction_select(WNR2000V5_GPIO_LED_LAN4, true);

	ath79_gpio_output_select(WNR2000V5_GPIO_LED_WAN,
			QCA953X_GPIO_OUT_MUX_LED_LINK5);
	ath79_gpio_output_select(WNR2000V5_GPIO_LED_LAN1,
			QCA953X_GPIO_OUT_MUX_LED_LINK1);
	ath79_gpio_output_select(WNR2000V5_GPIO_LED_LAN2,
			QCA953X_GPIO_OUT_MUX_LED_LINK2);
	ath79_gpio_output_select(WNR2000V5_GPIO_LED_LAN3,
			QCA953X_GPIO_OUT_MUX_LED_LINK3);
	ath79_gpio_output_select(WNR2000V5_GPIO_LED_LAN4,
			QCA953X_GPIO_OUT_MUX_LED_LINK4);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(wnr2000v5_leds_gpio),
			wnr2000v5_leds_gpio);
	ath79_register_gpio_keys_polled(-1, WNR2000V5_KEYS_POLL_INTERVAL,
			ARRAY_SIZE(wnr2000v5_gpio_keys),
			wnr2000v5_gpio_keys);
}

static void __init WNR2000V5_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000); // TODO: check the actual art location

  // LEDS
  wnr2000v5_gpio_led_setup();

  // SPI NOR
	ath79_register_m25p80(NULL);

  // Wifi
	ath79_wmac_set_led_pin(WNR2000V5_GPIO_LED_WAN);
	ath79_register_wmac(art + 0x1000, NULL);


  // USB port
	// ath79_register_usb(); // the usb port isn't used on this device

  ath79_register_mdio(0, 0x0);
	// ath79_register_mdio(1, 0x0); // TODO: What is that ?

  // LAN
	ath79_init_mac(ath79_eth0_data.mac_addr, art + 0, 0);
	ath79_init_mac(ath79_eth1_data.mac_addr, art + 6, 0);

	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
	ath79_eth0_data.speed = SPEED_100;
	ath79_eth0_data.duplex = DUPLEX_FULL;
	ath79_eth0_data.phy_mask = BIT(4);
	ath79_register_eth(0);

	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
	ath79_eth1_data.speed = SPEED_1000;
	ath79_eth1_data.duplex = DUPLEX_FULL;
	ath79_switch_data.phy_poll_mask |= BIT(4);
	ath79_switch_data.phy4_mii_en = 1;
	ath79_register_eth(1);

  /*
  ath79_setup_ar933x_phy4_switch(false, false);
  ath79_switch_data.phy4_mii_en = 1;
  */
}

MIPS_MACHINE(ATH79_MACH_WNR2000_V5, "WNR2000V5", "Netgear WNR2000V5 wifi router", WNR2000V5_setup);
