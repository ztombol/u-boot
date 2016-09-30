/*
 * Copyright (C) 2014 Samsung Electronics
 * Sanghee Kim <sh0130.kim@samsung.com>
 * Piotr Wilczek <p.wilczek@samsung.com>
 * Przemyslaw Marczak <p.marczak@samsung.com>
 *
 * Copyright (C) 2016 Zoltan Tombol <zoltan.tombol@gmail.com>
 *
 * Configuration settings for the Odroid-X2/U2/U3 (EXYNOS4412) boards.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * FIXME: USB and (thus?) Ethernet is broken. Cannot boot from a USB
 *        mass storage device. There is no power either. Cannot bring up
 *        the Ethernet interface as described in `doc/README.odroid'.
 *
 *        After fixing these issues, enable USB and Network boot. See
 *        the FIXMEs below.
 */

/*
 * Configuration
 * -------------
 *
 * These boards implement the generic distro configuration concept as
 * described in `doc/README.distro'.
 *
 * The following devices are searched for boot files.
 *
 *     1. eMMC module
 *     2. SD card
 *
 *
 * Bootable media layout
 * ---------------------
 *
 * The following table summarises the layout of the bootable media.
 * Offsets are given in 512 byte blocks. For eMMC the partition to which
 * a given offset applies to is given in addition.
 *
 *                |   SD   |       eMMC        |
 *                | offset | offset  partition |     size
 *     -----------+--------+-------------------+--------------
 *       bl1      |     1  |     0     boot    |  15 KiB
 *       bl2      |    31  |    30     boot    |  16 KiB
 *       uboot    |    63  |    62     boot    |   1 MiB
 *       tzsw     |  2111  |  2110     boot    | 156 KiB
 *       <unused> |     -  |     -     -       |  69 KiB
 *       env      |  2560  |  2560     user    |   4 KiB [1]
 *
 * [1]: Offset and size of the environment are defined in bytes by
 *      `CONFIG_ENV_OFFSET' and `CONFIG_ENV_SIZE', respectively.
 *
 *
 * Partition layout
 * ----------------
 *
 * DFU expects the following partition layout. However, the offset of
 * the first partition applies to any setup.
 *
 *       # | name | fs  | offset | contents
 *     ----+------+-----+--------+----------------------
 *       1 | boot | fat | 2 MiB  | kernel, fdt, initrd
 *       2 | root | any | -      | root filesystem
 *
 * It is important to leave enough, in this case 2 MiB, unused space
 * before the start of the first partition to avoid corrupting the
 * filesystem when saving the environment.
 */


#ifndef __CONFIG_ODROID_U3_H
#define __CONFIG_ODROID_U3_H

#include <configs/exynos4-common.h>
#include <linux/sizes.h>

#define CONFIG_SYS_L2CACHE_OFF
#ifndef CONFIG_SYS_L2CACHE_OFF
#define CONFIG_SYS_L2_PL310
#define CONFIG_SYS_PL310_BASE	0x10502000
#endif

#define CONFIG_MACH_TYPE	4289

#define CONFIG_NR_DRAM_BANKS	8
#define CONFIG_SYS_SDRAM_BASE	0x40000000
#define SDRAM_BANK_SIZE		SZ_256M
#define PHYS_SDRAM_1		CONFIG_SYS_SDRAM_BASE
/* Reserve the last 1 MiB for the secure firmware */
#define CONFIG_SYS_MEM_TOP_HIDE		SZ_1M
#define CONFIG_TZSW_RESERVED_DRAM_SIZE	CONFIG_SYS_MEM_TOP_HIDE

/* memtest works on */
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x5E00000)
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x3E00000)
#define CONFIG_SYS_TEXT_BASE		0x43e00000

/* select serial console configuration */
#define CONFIG_SERIAL1
#define CONFIG_BAUDRATE			115200

/* Console configuration */

#define CONFIG_BOOTARGS			"Please use defined boot"
#define CONFIG_DEFAULT_CONSOLE		"console=ttySAC1,115200n8\0"

#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_LOAD_ADDR \
					- GENERATED_GBL_DATA_SIZE)

#define CONFIG_SYS_MONITOR_BASE	0x00000000

#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		CONFIG_MMC_DEFAULT_DEV
#define CONFIG_ENV_SIZE			SZ_4K
#define CONFIG_ENV_OFFSET		(SZ_1K * 1280) /* 1.25 MiB offset */
#define CONFIG_ENV_OVERWRITE

/* Partitions name */
#define PARTS_BOOT		"boot"
#define PARTS_ROOT		"platform"

#define CONFIG_DFU_ALT \
	"uImage fat 0 1;" \
	"zImage fat 0 1;" \
	"Image.itb fat 0 1;" \
	"uInitrd fat 0 1;" \
	"exynos4412-odroidu3.dtb fat 0 1;" \
	"exynos4412-odroidx2.dtb fat 0 1;" \
	""PARTS_BOOT" part 0 1;" \
	""PARTS_ROOT" part 0 2\0" \

#define CONFIG_SET_DFU_ALT_INFO
#define CONFIG_SET_DFU_ALT_BUF_LEN	SZ_1K

#define CONFIG_DFU_ALT_BOOT_EMMC \
	"u-boot raw 0x3e 0x800 mmcpart 1;" \
	"bl1 raw 0x0 0x1e mmcpart 1;" \
	"bl2 raw 0x1e 0x1d mmcpart 1;" \
	"tzsw raw 0x83e 0x138 mmcpart 1\0"

#define CONFIG_DFU_ALT_BOOT_SD \
	"u-boot raw 0x3f 0x800;" \
	"bl1 raw 0x1 0x1e;" \
	"bl2 raw 0x1f 0x1d;" \
	"tzsw raw 0x83f 0x138\0"

/* Addresses where various images are loaded. */
#define CONFIG_LOADADDR		0x40007fc0
#define MEM_LAYOUT_ENV_SETTINGS \
	"fdt_addr_r=0x40800000\0"                          \
	"ramdisk_addr_r=0x42000000\0"                      \
	"kernel_addr_r=" __stringify(CONFIG_LOADADDR) "\0" \
	"pxefile_addr_r=0x42000000\0"                      \
	"scriptaddr=0x42000000\0"

/* Device Firmware Upgrade. */
#define BOOTENV_DFU \
	"dfu_alt_system=" CONFIG_DFU_ALT \
	"dfu_alt_info=Please reset the board\0"

/* Console settings and related command variables. */
#define BOOTENV_CONSOLE \
	"consoleon="                                     \
		"set console console=ttySAC1,115200n8; " \
		"save; "                                 \
		"reset; \0"                              \
	\
	"consoleoff="                                    \
		"set console console=ram; "              \
		"save; "                                 \
		"reset; \0"                              \
	\
	"console=" CONFIG_DEFAULT_CONSOLE

/* Generic distro configuration. See `doc/README.distro'. */
#ifndef CONFIG_SPL_BUILD

/* Define boot targets. */
#define BOOT_TARGET_DEVICES(func) \
        func(MMC, mmc, 0)    \
        func(MMC, mmc, 1)
/* FIXME: After fixing USB: Add USB and network boot targets. */
/*	func(USB, usb, 0)    \
        func(USB, usb, 1)    \
        func(PXE, pxe, na)   \
        func(DHCP, dhcp, na) */

#include <config_distro_defaults.h>
#include <config_distro_bootcmd.h>

#define CONFIG_EXTRA_ENV_SETTINGS \
	MEM_LAYOUT_ENV_SETTINGS \
	BOOTENV_DFU             \
	BOOTENV_CONSOLE         \
	BOOTENV                 \
	"bootdelay=0\0"
/*
 * FIXME: After fixing USB: Add network setup commands as described in
 *        `doc/README.distro' and `doc/README.odroid'.
 */
/*	"boot_net_usb_start="                           \
		"setenv usbethaddr 02:DE:AD:BE:EF:FF; " \
		"usb start; \0" */

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

#define CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS	3
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_SMSC95XX

/*
 * TODO: Add Odroid X support
 */
#define CONFIG_MISC_COMMON
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
#define CONFIG_BOARD_TYPES
#define CONFIG_MISC_INIT_R

#undef CONFIG_REVISION_TAG

#endif	/* __CONFIG_H */
