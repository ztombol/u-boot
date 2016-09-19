/*
 * Copyright (C) 2014 Samsung Electronics
 * Sanghee Kim <sh0130.kim@samsung.com>
 * Piotr Wilczek <p.wilczek@samsung.com>
 * Przemyslaw Marczak <p.marczak@samsung.com>
 *
 * Configuation settings for the Odroid-U3 (EXYNOS4412) board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_ODROID_U3_H
#define __CONFIG_ODROID_U3_H

#include <configs/exynos4-common.h>

#define CONFIG_IDENT_STRING 	" Arch Linux ARM"
#define CONFIG_SYS_L2CACHE_OFF
#ifndef CONFIG_SYS_L2CACHE_OFF
#define CONFIG_SYS_L2_PL310
#define CONFIG_SYS_PL310_BASE	0x10502000
#endif

#define CONFIG_MACH_TYPE	4289

#define CONFIG_NR_DRAM_BANKS	8
#define CONFIG_SYS_SDRAM_BASE	0x40000000
#define SDRAM_BANK_SIZE		(256 << 20)	/* 256 MB */
#define PHYS_SDRAM_1		CONFIG_SYS_SDRAM_BASE
/* Reserve the last 1 MiB for the secure firmware */
#define CONFIG_SYS_MEM_TOP_HIDE		(1UL << 20UL)
#define CONFIG_TZSW_RESERVED_DRAM_SIZE	CONFIG_SYS_MEM_TOP_HIDE

/* memtest works on */
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x5E00000)
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x3E00000)
#define CONFIG_SYS_TEXT_BASE		0x43e00000

#include <linux/sizes.h>

/* select serial console configuration */
#define CONFIG_SERIAL1
#define CONFIG_BAUDRATE			115200

/* Console configuration */
#define CONFIG_SYS_CONSOLE_INFO_QUIET
#define CONFIG_SYS_CONSOLE_IS_IN_ENV

#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_LOAD_ADDR \
					- GENERATED_GBL_DATA_SIZE)

#define CONFIG_SYS_MONITOR_BASE	0x00000000

#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		CONFIG_MMC_DEFAULT_DEV
#define CONFIG_ENV_SIZE			4096
#define CONFIG_ENV_OFFSET		(SZ_1K * 1280) /* 1.25 MiB offset */
#define CONFIG_ENV_OVERWRITE

/* Generic distro config. See `doc/README.distro' for details. */
#ifndef CONFIG_SPL_BUILD

#define CONFIG_LOADADDR		0x40007FC0

/*
 * Memory layout. Addresses where various images are loaded.
 *
 *   fdt_addr_r     - Flattened Device Tree
 *   ramdisk_addr_r - Initial Ramdisk
 *   kernel_addr_r  - Kernel
 *   pxefile_addr_r - PXE configuration file and `extlinux.conf'
 *   scriptaddr     - Boot script `boot.scr'
 */
#define MEM_LAYOUT_ENV_SETTINGS \
	"fdt_addr_r=0x40800000\0"                          \
	"ramdisk_addr_r=0x42000000\0"                      \
	"kernel_addr_r=" __stringify(CONFIG_LOADADDR) "\0" \
	"pxefile_addr_r=0x42000000\0"                      \
	"scriptaddr=0x42000000\0"

#include <config_distro_defaults.h>

#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0)    \
	func(MMC, mmc, 1)

#include <config_distro_bootcmd.h>

/*
 * Bootable media layout:
 * dev:    SD   eMMC(part boot)
 * BL1      1    0
 * BL2     31   30
 * UBOOT   63   62
 * TZSW  2111 2110
 * ENV   2560 2560(part user)
*/
#define CONFIG_EXTRA_ENV_SETTINGS \
	MEM_LAYOUT_ENV_SETTINGS \
	BOOTENV

#endif /* CONFIG_SPL_BUILD */

/* I2C */
#define CONFIG_SYS_I2C_S3C24X0
#define CONFIG_SYS_I2C_S3C24X0_SPEED	100000
#define CONFIG_SYS_I2C_S3C24X0_SLAVE	0

/* GPT */
#define CONFIG_RANDOM_UUID

/* Security subsystem - enable hw_rand() */
#define CONFIG_EXYNOS_ACE_SHA
#define CONFIG_LIB_HW_RAND

/* USB */
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_EXYNOS
#define CONFIG_USB_STORAGE

#define CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS	3
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_SMSC95XX

/*
 * Supported Odroid boards: X3, U3
 * TODO: Add Odroid X support
 */
#define CONFIG_MISC_COMMON
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
#define CONFIG_BOARD_TYPES
#define CONFIG_MISC_INIT_R

#undef CONFIG_REVISION_TAG

#endif	/* __CONFIG_H */
