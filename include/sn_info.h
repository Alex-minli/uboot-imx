/*
 * Common sn info paraments define
 *
 * Copyright (C) 2010-2014 Pateo Shanghai, Inc.
 * Min li <minli@pateo.com.cn>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
// minli-port-181105
 */

#ifndef _SN_INFO_H_
#define _SN_INFO_H_

/* SN location define(Nor flash) */
#define	SN_Start_addr		0x7C0000
#define	SN_Size			0x020000

#define	SN_A_Start_Offset	0x00000
#define	SN_B_Start_Offset	0x20000

/* boot set paraments define */
#define	SN_MBSN			0x00
#define	SN_PDSN			0x01
#define	SN_REVERSED		0x02
#define	SN_BTAD			0x03
#define	SN_WIFI			0x04

#define	SN_MBSN_START		0x00
#define	SN_MBSN_SIZE		0x20
#define	SN_PDSN_START		0x20
#define	SN_PDSN_SIZE		0x20
#define	SN_REVERSED_START	0x40
#define	SN_REVERSED_SIZE	0x20
#define	SN_BTAD_START		0x60
#define	SN_BTAD_SIZE		0x20
#define	SN_WIFI_START		0x80
#define	SN_WIFI_SIZE		0x20

#define	REVERSED_SIZE		SN_Size - SN_MBSN_SIZE - SN_PDSN_SIZE - SN_REVERSED_SIZE - SN_BTAD_SIZE - SN_WIFI_SIZE

struct sn_info {
	char mbsn_A[SN_MBSN_SIZE];
	char PDSN_A[SN_PDSN_SIZE];
	char reversed_A[SN_REVERSED_SIZE];
	char BTAD_A[SN_BTAD_SIZE];
	char WIFI_A[SN_WIFI_SIZE];
	char reversed0[REVERSED_SIZE];

	char mbsn_B[SN_MBSN_SIZE];
	char PDSN_B[SN_PDSN_SIZE];
	char reversed_B[SN_REVERSED_SIZE];
	char BTAD_B[SN_BTAD_SIZE];
	char WIFI_B[SN_WIFI_SIZE];
	char reversed1[REVERSED_SIZE];
};

extern struct boot_param_info boot_param;
struct boot_param_info *get_bparam_info(void);

#endif /* _SN_INFO_H_ */
