/*
 *
 * Copyright (C) 2010-2014 Pateo Shanghai, Inc.
 * Min li <minli@pateo.com.cn>
 *
 *
 */
// minli-port-181105

#include <common.h>
#include <command.h>
#include <sn_info.h>
#include <environment.h>


#ifdef CONFIG_CMD_SNINFO
static struct sn_info *sn_info;

struct sn_info *get_sn_info(void)
{
	int ret = 1;

	unsigned int sninfo_addr;
	const char *addr_str;
	char op_cmd[128];

// Check Env
	addr_str = getenv("sninfo_loadaddr");
	if (addr_str != NULL)
		sninfo_addr = simple_strtoul(addr_str, NULL, 16);
	else
		sninfo_addr = SN_Start_addr;


// Probe SPI
	sprintf(op_cmd, "sf probe");

	debug("Run Command '%s'\n", op_cmd);
	ret = run_command(op_cmd, 0);
	if (ret) {
		printf("FAIL:Init of Nor Flash");

		return (struct sn_info *)-1;
	} else {
		sprintf(op_cmd, "sf read 0x%x 0x%x 0x%x",
				CONFIG_LOADADDR, /* dest   */
				sninfo_addr, 	 /* source */
				SN_Size * 2 	 /* length */);

		debug("Run Command '%s'\n", op_cmd);
		ret = run_command(op_cmd, 0);
		if (ret) {
			debug("Read sn info Error!\n");

			return (struct sn_info *)-1;
		} else
			return (struct sn_info *)CONFIG_LOADADDR;
	}
}

static int set_sn_info(struct sn_info *sninfo_buffer)
{
	int ret = 1;

	unsigned int sninfo_addr;
	const char *addr_str;
	char op_cmd[128];

// Check Env
	addr_str = getenv("sninfo_loadaddr");
	if (addr_str != NULL)
		sninfo_addr = simple_strtoul(addr_str, NULL, 16);
	else
		sninfo_addr = SN_Start_addr;

// Erase Flash
	sprintf(op_cmd, "sf erase 0x%x 0x%x",
			sninfo_addr, /*dest*/
			SN_Size * 2  /*length*/);

	debug("Run Command '%s'\n", op_cmd);
	ret = run_command(op_cmd, 0);
	if (ret) {
		debug("Erase flash Error!");

		return ret;
	}
	else {
		debug("Erase OKAY!");
	
// write Flash
		sprintf(op_cmd, "sf write 0x%x 0x%x 0x%x",
				CONFIG_LOADADDR, /* dest   */
				sninfo_addr, 	 /* source */
				SN_Size * 2  	 /* length */);

		debug("Run Command '%s'\n", op_cmd);
		ret = run_command(op_cmd, 0);
		if (ret) {
			debug("Read sn info Error!\n");

			return ret;
		} else
			return 0;
	}
}


static int do_sninfo_clear(int argc, char * const argv[])
{
	int ret = 1;
	const char *op_type;

	op_type = argv[2];

	if (strcmp(op_type, "mbsn") == 0) {
		sn_info = get_sn_info();

		memset(sn_info->mbsn_A, 0xFF , SN_MBSN_SIZE);
		memset(sn_info->mbsn_B, 0xFF , SN_MBSN_SIZE);

		ret = set_sn_info(sn_info);
		printf("Reset mbsn.\n");
	} else if (strcmp(op_type, "pdsn") == 0) {
		sn_info = get_sn_info();

		memset(sn_info->PDSN_A, 0xFF , SN_PDSN_SIZE);
		memset(sn_info->PDSN_B, 0xFF , SN_PDSN_SIZE);

		ret = set_sn_info(sn_info);
		printf("Reset PDSN.\n");
	} else if (strcmp(op_type, "BTAD") == 0) {
		sn_info = get_sn_info();

		memset(sn_info->BTAD_A, 0xFF , SN_BTAD_SIZE);
		memset(sn_info->BTAD_B, 0xFF , SN_BTAD_SIZE);

		ret = set_sn_info(sn_info);
		printf("Reset BT MAC address.\n");
	} else if (strcmp(op_type, "WIFI") == 0) {
		sn_info = get_sn_info();

		memset(sn_info->WIFI_A, 0xFF , SN_WIFI_SIZE);
		memset(sn_info->WIFI_B, 0xFF , SN_WIFI_SIZE);

		ret = set_sn_info(sn_info);
		printf("Reset WIFI MAC address.\n");
	}


	return ret;
}

static int do_sninfo_show(int argc, char * const argv[])
{
	sn_info = get_sn_info();

	printf("mbsn:         %s\n", sn_info->mbsn_A[0] == 0xFF ? "" : sn_info->mbsn_A);
	printf("PDSN:         %s\n", sn_info->PDSN_A[0] == 0xFF ? "" : sn_info->PDSN_A);
	printf("BT address:   %s\n", sn_info->BTAD_A[0] == 0xFF ? "" : sn_info->BTAD_A);
	printf("Wifi address: %s\n", sn_info->WIFI_A[0] == 0xFF ? "" : sn_info->WIFI_A);

	return 0;
}

static int do_sninfo_set(int argc, char * const argv[])
{
	const char *op_type;

	op_type = argv[2];

	if (strcmp(op_type, "mbsn") == 0) {
		sn_info = get_sn_info();

		sprintf(sn_info->mbsn_A, argv[3]);
		sprintf(sn_info->mbsn_B, argv[3]);

		debug("Set mbsn to \"%s\"\n", sn_info->mbsn_A);

		return(set_sn_info(sn_info));
	} else if (strcmp(op_type, "pdsn") == 0) {
		sn_info = get_sn_info();

		sprintf(sn_info->PDSN_A, argv[3]);
		sprintf(sn_info->PDSN_B, argv[3]);

		debug("Set PDSN to \"%s\"\n", sn_info->PDSN_A);

		return(set_sn_info(sn_info));
	} else if (strcmp(op_type, "BTAD") == 0) {
		sn_info = get_sn_info();

		sprintf(sn_info->BTAD_A, argv[3]);
		sprintf(sn_info->BTAD_B, argv[3]);

		debug("Set BT MAC address to \"%s\"\n", sn_info->BTAD_A);

		return(set_sn_info(sn_info));
	} else if (strcmp(op_type, "WIFI") == 0) {
		sn_info = get_sn_info();

		sprintf(sn_info->WIFI_A, argv[3]);
		sprintf(sn_info->WIFI_B, argv[3]);

		debug("Set WIFI MAC address to \"%s\"\n", sn_info->WIFI_A);

		return(set_sn_info(sn_info));
	}
	else
		return -1;

}

int do_sninfo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	const char *cmd;
	int ret;

	/* need at least two arguments */
	if (argc < 2)
		goto usage;

	cmd = argv[1];

	if (strcmp(cmd, "clear") == 0) {
		ret = do_sninfo_clear(argc, argv);
		goto done;
	}
	else if (strcmp(cmd, "show") == 0) {
		ret = do_sninfo_show(argc, argv);
		goto done;
	}
	else if (strcmp(cmd, "set") == 0) {
		ret = do_sninfo_set(argc, argv);
		goto done;
	}
	else
		ret = -1;

done:
	if (ret != -1)
		return ret;

usage:
	return CMD_RET_USAGE;
}

U_BOOT_CMD(
	sninfo,	4,	1,	do_sninfo,
	"Set and clear SN information",
	"clear mbsn      - Clear SN of MB\n"
	"sninfo clear pdsn      - Clear PDSN\n"
	"sninfo clear btad      - Clear BT mac address\n"
	"sninfo clear wifi      - Clear WIFI mac address\n"
	"sninfo set mbsn        - Set SN of MB\n"
	"sninfo set pdsn        - Set PDSN\n"
	"sninfo set btad        - Set BT mac address\n"
	"sninfo set wifi        - Set WIFI mac address\n"
	"sninfo show            - Show SN information\n"
);


#endif	/* CONFIG_CMD_SNINFO */
