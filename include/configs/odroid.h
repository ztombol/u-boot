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
 *        the FIXMEs in the Environment section below.
 *
 * TODO: Add Odroid-X support. The X uses a variant of the same
 *       EXYNOS4412 SoC that packages only 1GiB of RAM.
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

/* Board ID for Linux: ODROID-X. */
#define CONFIG_MACH_TYPE	4289


/*----------------------------------------------------------------------
 * Hardware setup
 *--------------------------------------------------------------------*/

/* Disable L2 cache. */
#define CONFIG_SYS_L2CACHE_OFF
#ifndef CONFIG_SYS_L2CACHE_OFF
#define CONFIG_SYS_L2_PL310
#define CONFIG_SYS_PL310_BASE	0x10502000
#endif

/* Enable I2C. */
#define CONFIG_SYS_I2C_S3C24X0
#define CONFIG_SYS_I2C_S3C24X0_SPEED	100000
#define CONFIG_SYS_I2C_S3C24X0_SLAVE	0

/* Enable hardware random number generator. */
#define CONFIG_EXYNOS_ACE_SHA
#define CONFIG_LIB_HW_RAND

/* Enable USB EHCI controller. */
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_EXYNOS
#define CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS	3

/* Enable USB host networking. */
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_SMSC95XX


/*----------------------------------------------------------------------
 * Physical Memory Map
 *--------------------------------------------------------------------*/

/* A total of 2 GiB memory in 8 banks. */
#define CONFIG_NR_DRAM_BANKS	8
#define SDRAM_BANK_SIZE		SZ_256M

/* Start address of SDRAM. */
#define CONFIG_SYS_SDRAM_BASE	0x40000000
#define PHYS_SDRAM_1		CONFIG_SYS_SDRAM_BASE

/*
 * FIXME: Move to a newer method of memory testing! This method has been
 *        deprecated and replaced by faster and more effective
 *        alternatives.
 *
 *        Use either `get_ram_size()', or testing implemented by the
 *        POST sub-system. See `doc/README.memory-test' for more.
 */
/* The `mtest' command tests this region of memory. */
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x5E00000)

/* Start address of U-Boot before relocation. */
#define CONFIG_SYS_TEXT_BASE	0x43e00000

/* Fallback default load address after `loadaddr' environment var. */
#define CONFIG_SYS_LOAD_ADDR	(CONFIG_SYS_SDRAM_BASE + 0x3e00000)

/* Initial stack pointer (temporary stack before relocation). */
#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_LOAD_ADDR \
					- GENERATED_GBL_DATA_SIZE)

/* Reserve the top 1 MiB for the secure firmware (TrustZone SW). */
#define CONFIG_SYS_MEM_TOP_HIDE		SZ_1M
#define CONFIG_TZSW_RESERVED_DRAM_SIZE	CONFIG_SYS_MEM_TOP_HIDE

/* Start address of boot monitor code. */
#define CONFIG_SYS_MONITOR_BASE	0x00000000


/*----------------------------------------------------------------------
 * Device Firmware Upgrade
 *--------------------------------------------------------------------*/

/*
 * Set `dfu_alt_info' at runtime by concatenating `dfu_alt_boot' and
 * `dfu_alt_system'. See `doc/README.odroid' for more.
 */
#define CONFIG_SET_DFU_ALT_INFO
#define CONFIG_SET_DFU_ALT_BUF_LEN	SZ_1K

/* Partition names */
#define PARTS_BOOT	"boot"
#define PARTS_ROOT	"root"

/* Value of `dfu_alt_system' set by `CONFIG_EXTRA_ENV_SETTINGS'. */
#define CONFIG_DFU_ALT_SYSTEM \
	"uImage fat 0 1; "                   \
	"zImage fat 0 1; "                   \
	"Image.itb fat 0 1; "                \
	"uInitrd fat 0 1; "                  \
	"exynos4412-odroidu3.dtb fat 0 1; "  \
	"exynos4412-odroidx2.dtb fat 0 1; "  \
	""PARTS_BOOT" part 0 1; "            \
	""PARTS_ROOT" part 0 2; \0"

/* Value of `dfu_alt_boot' when booting from an eMMC module. */
#define CONFIG_DFU_ALT_BOOT_EMMC \
	"u-boot raw 0x3e 0x800 mmcpart 1; "  \
	"bl1 raw 0x0 0x1e mmcpart 1; "       \
	"bl2 raw 0x1e 0x1d mmcpart 1; "      \
	"tzsw raw 0x83e 0x138 mmcpart 1; \0"

/* Value of `dfu_alt_boot' when booting from an SD card. */
#define CONFIG_DFU_ALT_BOOT_SD \
	"u-boot raw 0x3f 0x800; "            \
	"bl1 raw 0x1 0x1e; "                 \
	"bl2 raw 0x1f 0x1d; "                \
	"tzsw raw 0x83f 0x138; \0"


/*----------------------------------------------------------------------
 * Console
 *--------------------------------------------------------------------*/

/* Select and configure serial console. */
#define CONFIG_SERIAL1
#define CONFIG_BAUDRATE		115200

/* Default settings for the environment (e.g. kernel parameters). */
#define CONFIG_DEFAULT_CONSOLE	"ttySAC1," __stringify(CONFIG_BAUDRATE) "n8"


/*----------------------------------------------------------------------
 * Misc settings
 *--------------------------------------------------------------------*/

/*
 * Generate a random UUID for every partition that does not have one
 * specified in the environment. See `doc/README.gpt' for more.
 */
#define CONFIG_RANDOM_UUID


/*----------------------------------------------------------------------
 * Environment
 *--------------------------------------------------------------------*/

/* Enable storing the environment in MMC. */
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		CONFIG_MMC_DEFAULT_DEV
#define CONFIG_ENV_OFFSET		(SZ_1K * 1280) /* 1.25 MiB */
#define CONFIG_ENV_SIZE			SZ_4K
#define CONFIG_ENV_OVERWRITE

/*
 * Define the following environment variables:
 *   - soc_rev
 *   - soc_id    = 4412
 *   - boardname = odroid{u3,x2}
 *   - fdtfile   = exynos4412-odroid{u3,x2}
 */
#define CONFIG_MISC_COMMON
#define CONFIG_MISC_INIT_R
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
#define CONFIG_BOARD_TYPES
#undef CONFIG_REVISION_TAG

/*
 * Define `bootargs', the environment variable specifying kernel
 * parameters. This serves only as an error message to users and
 * developers that they should have set this in `extlinux.conf',
 * `boot.scr' or `CONFIG_EXTRA_ENV_SETTINGS' below.
 */
#define CONFIG_BOOTARGS		"Please use defined boot"

/* Define `loadaddr', the default load address. */
#define CONFIG_LOADADDR		0x40007fc0

/*
 * Addresses where various images are loaded.
 *
 *   fdt_addr_r     - Flattened Device Tree
 *   ramdisk_addr_r - Initial Ramdisk
 *   kernel_addr_r  - Kernel
 *   pxefile_addr_r - PXE configuration file and `extlinux.conf'
 *   scriptaddr     - Boot script `boot.scr'
 *
 * See `doc/README.distro' for details.
 */
#define MEM_LAYOUT_ENV_SETTINGS \
	"fdt_addr_r=0x40800000\0"                          \
	"ramdisk_addr_r=0x42000000\0"                      \
	"kernel_addr_r=" __stringify(CONFIG_LOADADDR) "\0" \
	"pxefile_addr_r=0x42000000\0"                      \
	"scriptaddr=0x42000000\0"

/* Device Firmware Upgrade. */
#define BOOTENV_DFU \
	"dfu_alt_system=" CONFIG_DFU_ALT_SYSTEM \
	"dfu_alt_info=Please reset the board\0"

/* Console settings and related command variables. */
#define BOOTENV_CONSOLE \
	"consoleon="                                               \
		"set console console=" CONFIG_DEFAULT_CONSOLE "; " \
		"save; "                                           \
		"reset; \0"                                        \
	\
	"consoleoff="                                              \
		"set console console=ram; "                        \
		"save; "                                           \
		"reset; \0"                                        \
	\
	"console=" CONFIG_DEFAULT_CONSOLE "\0"

/* Generic distro configuration. See `doc/README.distro'. */
#ifndef CONFIG_SPL_BUILD

/* Define boot targets. */
#define BOOT_TARGET_DEVICES(func) \
        func(MMC, mmc, 0) /* eMMC module */ \
        func(MMC, mmc, 1) /* SD card */
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

#endif	/* __CONFIG_H */
