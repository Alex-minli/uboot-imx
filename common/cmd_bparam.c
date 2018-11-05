/*
 *
 * Copyright (C) 2010-2014 Pateo Shanghai, Inc.
 * Min li <minli@pateo.com.cn>
 *
 *
 */
// minli-port-181011

#include <common.h>
#include <command.h>
#include <bparam.h>
#include <fsl_fastboot.h>
#include <environment.h>
#include <mmc.h>


#ifdef CONFIG_CMD_BPARAM
static struct boot_param_info *bparam_info;

struct boot_param_info *get_bparam_info(void)
{
	int ret = 1;

	unsigned long boot_param_addr;
	const char *addr_str;

	char *bparam_cmd;

// Check Env
	addr_str = getenv("params_loadaddr");
	if (addr_str != NULL)
		boot_param_addr = simple_strtoul(addr_str, NULL, 16);
	else
		boot_param_addr = CONFIG_PARAMS_LOADADDR;

	bparam_cmd = getenv("boot_judge");
	debug("Run Command '%s'\n", bparam_cmd);

	ret = run_command(bparam_cmd, 0);
	if (ret){
		debug("Read boot param Error!\n");
		return (struct boot_param_info *)-1;
	}else {
		struct boot_param_info *bparam_buffer;
		bparam_buffer = (struct boot_param_info *)boot_param_addr;

		if( strlen(bparam_buffer->Boot_type) <= BOOT_TYPE_SIZE && 
		    strlen(bparam_buffer->SystemA_Version) <= SYSTEM_VERSION_SIZE &&
		    strlen(bparam_buffer->SystemB_Version) <= SYSTEM_VERSION_SIZE && 
		    strlen(bparam_buffer->Boot_cmd) <= BOOT_CMD_SIZE &&
		    strlen(bparam_buffer->System_type) <= SYSTEM_TYPE_SIZE)

		debug("OKAY!\n");
		debug(" Check boot cmd:    %d-\n",memcmp(bparam_buffer->Boot_cmd, "RECOVERY--", 10));

//		memcpy(&boot_param, bparam_buffer, sizeof(struct boot_param_info));

		return (struct boot_param_info *)boot_param_addr;
	}
}

static int set_bparam_info(struct boot_param_info *bparam_buffer)
{
	int ret = 1;

	unsigned long boot_param_addr;
	const char *addr_str;

	char *bparam_cmd;

// Check Env
	addr_str = getenv("params_loadaddr");
	if (addr_str != NULL)
		boot_param_addr = simple_strtoul(addr_str, NULL, 16);
	else
		boot_param_addr = CONFIG_PARAMS_LOADADDR;

// Update write buffer
	memcpy((void *)boot_param_addr,	bparam_buffer, sizeof(struct boot_param_info));

	bparam_cmd = getenv("boot_type_set");
	debug("Run Command '%s'\n", bparam_cmd);

		
	ret = run_command(bparam_cmd, 0);
	if (ret)
		debug("Save boot param Error!");
	else
		debug("OKAY!");
	

	return ret;
}


static int do_mmc_bparam_clear(int argc, char * const argv[])
{
	int ret = 1;

	bparam_info = get_bparam_info();

	memset(bparam_info->Boot_type, 0 , BOOT_TYPE_SIZE);
	memset(bparam_info->Boot_cmd, 0 , BOOT_CMD_SIZE);

	sprintf(bparam_info->Boot_type, "SYSTEMA");

	setenv("bootcmd_android_recovery", "");
	setenv("bootcmd", "boota mmc1");

	printf("Reset boot type to \"%s\" & clear boot command.\n", bparam_info->Boot_type);

	ret = set_bparam_info(bparam_info);

	return ret;
}

static int do_mmc_bparam_show(int argc, char * const argv[])
{
	bparam_info = get_bparam_info();

	printf("Boot type: %s\n", bparam_info->Boot_type);
	printf("Boot cmd: %s\n", bparam_info->Boot_cmd);
	printf("System type: %s\n", bparam_info->System_type);

	return 0;
}

static int do_mmc_bparam_set(int argc, char * const argv[])
{
	const char *cmd_argv;

	cmd_argv = argv[2];
	bparam_info = get_bparam_info();

	if (strcmp(cmd_argv, "systema") == 0) {
		memset(bparam_info->Boot_type, 0 , BOOT_TYPE_SIZE);
		sprintf(bparam_info->Boot_type, "SYSTEMA");

		memset(bparam_info->Boot_cmd, 0 , BOOT_CMD_SIZE);

		setenv("bootcmd_android_recovery", "");
		setenv("bootcmd", "boota mmc1");

		debug("Reset boot type to \"%s\"\n", bparam_info->Boot_type);

		return(set_bparam_info(bparam_info));
	}
	else if (strcmp(cmd_argv, "systemb") == 0) {
		memset(bparam_info->Boot_type, 0 , BOOT_TYPE_SIZE);
		sprintf(bparam_info->Boot_type, "SYSTEMB");
		
		memset(bparam_info->Boot_cmd, 0 , BOOT_CMD_SIZE);

		setenv("bootcmd_android_recovery", "");
		setenv("bootcmd", "boota mmc1 bootb");

		debug("Reset boot type to \"%s\"\n", bparam_info->Boot_type);

		return(set_bparam_info(bparam_info));
	}
	else if (strcmp(cmd_argv, "recovery") == 0) {
		memset(bparam_info->Boot_type, 0 , BOOT_TYPE_SIZE);
		sprintf(bparam_info->Boot_type, "SYSTEMA");

		memset(bparam_info->Boot_cmd, 0 , BOOT_CMD_SIZE);
		sprintf(bparam_info->Boot_cmd, "RECOVERY--");

		setenv("bootcmd_android_recovery",
			"boota mmc1 recovery");
		setenv("bootcmd", "run bootcmd_android_recovery");

		debug("Reset boot type to \"%s\"\n", bparam_info->Boot_type);
		debug("Set boot cmdline to: %s\n", bparam_info->Boot_cmd);

		return(set_bparam_info(bparam_info));
	}
	else if (strcmp(cmd_argv, "recoveryb") == 0) {
		memset(bparam_info->Boot_type, 0 , BOOT_TYPE_SIZE);
		sprintf(bparam_info->Boot_type, "SYSTEMB");

		memset(bparam_info->Boot_cmd, 0 , BOOT_CMD_SIZE);
		sprintf(bparam_info->Boot_cmd, "RECOVERY--");

		setenv("bootcmd_android_recovery",
			"boota mmc1 recoveryb");
		setenv("bootcmd", "run bootcmd_android_recovery");

		debug("Reset boot type to \"%s\"\n", bparam_info->Boot_type);
		debug("Set boot cmdline to: %s\n", bparam_info->Boot_cmd);

		return(set_bparam_info(bparam_info));
	}
	else if (strcmp(cmd_argv, "wipe_data") == 0) {
		memset(bparam_info->Boot_cmd, 0 , BOOT_CMD_SIZE);
		sprintf(bparam_info->Boot_cmd, "RECOVERY--wipe_data");

		if( strcmp(bparam_info->Boot_type, "SYSTEMA") == 0)
			setenv("bootcmd_android_recovery",
				"boota mmc1 recovery");
		else if( strcmp(bparam_info->Boot_type, "SYSTEMB") == 0)
			setenv("bootcmd_android_recovery",
				"boota mmc1 recoveryb");

		setenv("bootcmd", "run bootcmd_android_recovery");

		debug("Set boot cmdline to: %s\n", bparam_info->Boot_cmd);

		return(set_bparam_info(bparam_info));
	}
	else
		return -1;

}

static int do_mmc_bparam_cmd(int argc, char * const argv[])
{
	int ret = 1;
	const char *cmd_argv;

	if (argc < 3){
		printf("Cmd without argument.\n");
		return -1;
	}

	cmd_argv = argv[2];

	bparam_info = get_bparam_info();
	memset(bparam_info->Boot_cmd, 0 , BOOT_CMD_SIZE);

	if( strlen(cmd_argv) < BOOT_CMD_SIZE ){
		sprintf(bparam_info->Boot_cmd, cmd_argv);
		debug("Set boot cmdline to: %s\n", bparam_info->Boot_cmd);

		ret = set_bparam_info(bparam_info);
	}
	else {
		printf("Cmdline is toolarge\n");

		ret = -1;
	}

	return ret;
}

int do_param(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	const char *cmd;
	int ret;

	/* need at least two arguments */
	if (argc < 2)
		goto usage;

	cmd = argv[1];

	if (strcmp(cmd, "clear") == 0) {
		ret = do_mmc_bparam_clear(argc, argv);
		goto done;
	}
	else if (strcmp(cmd, "show") == 0) {
		ret = do_mmc_bparam_show(argc, argv);
		goto done;
	}
	else if (strcmp(cmd, "set") == 0) {
		ret = do_mmc_bparam_set(argc, argv);
		goto done;
	}
	else if (strcmp(cmd, "cmd") == 0) {
		ret = do_mmc_bparam_cmd(argc, argv);
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
	bparam,	3,	1,	do_param,
	"Set and clear boot param",
	"clear    - Clear boot param. Reset to system A\n"
	"bparam set systema     - Boot from system A\n"
	"bparam set systemb     - Boot from system B\n"
	"bparam set recovery    - Boot to Recovery A\n"
	"bparam set recoveryb   - Boot to Recovery B\n"
	"bparam set wipe_data   - Boot to Recovery and clear data & private\n"
	"bparam cmd [cmd]       - Boot accroding to [cmd]\n"
	"bparam show            - Show Boot paraments\n"
);


#endif	/* CONFIG_CMD_BPARAM */
