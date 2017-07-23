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
#include "dev-wmac.h"
#include "machtypes.h"

#define WNR2000V5_GPIO_LED_SYSTEM	13
#define WNR2000V5_GPIO_BTN_RESET	11

#define WNR2000V5_KEYS_POLL_INTERVAL	20	/* msecs */
#define WNR2000V5_KEYS_DEBOUNCE_INTERVAL	(3 * WNR2000V5_KEYS_POLL_INTERVAL)

static struct gpio_led WNR2000V5_leds_gpio[] __initdata = {
	{
		.name		= "tp-link:blue:system",
		.gpio		= WNR2000V5_GPIO_LED_SYSTEM,
		.active_low	= 1,
	},
};

static struct gpio_keys_button WNR2000V5_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = WNR2000V5_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= WNR2000V5_GPIO_BTN_RESET,
		.active_low	= 0,
	}
};


static void __init WNR2000V5_setup(void)
{
  u8 *mac = (u8 *) KSEG1ADDR(0x1f01fc00);
  u8 *ee = (u8 *) KSEG1ADDR(0x1fff1000);
  u8 tmpmac[ETH_ALEN];

	ath79_register_m25p80(NULL);
  ath79_setup_ar933x_phy4_switch(false, false);

  ath79_register_mdio(0, 0x0);

  /* LAN */
  ath79_switch_data.phy4_mii_en = 1;
  ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
  ath79_eth0_data.duplex = DUPLEX_FULL;
  ath79_eth0_data.speed = SPEED_100;
  ath79_eth0_data.phy_mask = BIT(4);
  ath79_init_mac(ath79_eth0_data.mac_addr, mac, 1);
  ath79_register_eth(0);

  ath79_init_mac(tmpmac, mac, 0);
  ath79_register_wmac(ee, tmpmac);
	ath79_register_leds_gpio(-1, ARRAY_SIZE(WNR2000V5_leds_gpio),
		WNR2000V5_leds_gpio);

	ath79_register_gpio_keys_polled(1, WNR2000V5_KEYS_POLL_INTERVAL,
		ARRAY_SIZE(WNR2000V5_gpio_keys),
		WNR2000V5_gpio_keys);

}

MIPS_MACHINE(ATH79_MACH_WNR2000_V5, "WNR2000V5", "Netgear WNR2000V5 wifi router", WNR2000V5_setup);
