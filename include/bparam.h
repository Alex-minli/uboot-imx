/*
 * Common boot paraments define
 *
 * Copyright (C) 2010-2014 Pateo Shanghai, Inc.
 * Min li <minli@pateo.com.cn>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
// minli-port-181011
 */

#ifndef _BOOT_PARAM_H_
#define _BOOT_PARAM_H_

/* paraments location define */
#define	Paraments_Start_Sector	0x7800
#define	Paraments_Size		0x01

/* boot set paraments define */
#define	BOOT_PARAM_SYSTEMA	0x00
#define	BOOT_PARAM_SYSTEMB	0x01
#define	BOOT_PARAM_RECOVERY	0x02

/* Enum list - set commands
enum boot_type {
	BOOT_PARAM_SYSTEMA,
	BOOT_PARAM_SYSTEMB,
	BOOT_PARAM_RECOVERY,
};
 */

#define	BOOT_TYPE_START		0x00
#define	BOOT_TYPE_SIZE		0x20
#define	SYSTEMA_VERSION_START	0x20
#define	SYSTEMB_VERSION_START	0x52
#define	SYSTEM_VERSION_SIZE	0x32
#define	BOOT_CMD_START		0x84
#define	BOOT_CMD_SIZE		0x15C
#define	SYSTEM_TYPE_START	0x1E0
#define	SYSTEM_TYPE_SIZE	0x20

struct boot_param_info {
	char Boot_type[BOOT_TYPE_SIZE];
	char SystemA_Version[SYSTEM_VERSION_SIZE];
	char SystemB_Version[SYSTEM_VERSION_SIZE];
	char Boot_cmd[BOOT_CMD_SIZE];
	char System_type[SYSTEM_TYPE_SIZE];
};

extern struct boot_param_info boot_param;
struct boot_param_info *get_bparam_info(void);

#endif /* _BOOT_PARAM_H_ */
