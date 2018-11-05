/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/errno.h>
#include <asm/imx-common/video.h>

int board_video_skip(void)
{
// minli-debug_video
	debug("%s: Total display count = %d\n", __func__, display_count);

	int i;
	int ret;
	char const *panel = getenv("panel");

	if (!panel) {
// minli-debug_video
		debug("\t No environment variable defined, detect assambled panel\n");
		for (i = 0; i < display_count; i++) {
			struct display_info_t const *dev = displays+i;
			if (dev->detect && dev->detect(dev)) {
				panel = dev->mode.name;
				printf("auto-detected panel %s\n", panel);
				break;
			}
		}
		if (!panel) {
			panel = displays[0].mode.name;
			printf("No panel detected: default to %s\n", panel);
			i = 0;
		}
	} else {
// minli-debug_video
		debug("\t Environment variable defined.\n");
		for (i = 0; i < display_count; i++) {
			if (!strcmp(panel, displays[i].mode.name))
				break;
		}
	}

	if (i < display_count) {
		ret = ipuv3_fb_init(&displays[i].mode, 0,
				    displays[i].pixfmt);
		if (!ret) {
			if (displays[i].enable)
				displays[i].enable(displays + i);

			printf("Display: %s (%ux%u)\n",
			       displays[i].mode.name,
			       displays[i].mode.xres,
			       displays[i].mode.yres);
		} else
			printf("LCD %s cannot be configured: %d\n",
			       displays[i].mode.name, ret);
	} else {
		printf("unsupported panel %s\n", panel);
		return -EINVAL;
	}

	return 0;
}

#ifdef CONFIG_IMX_HDMI
#include <asm/arch/mxc_hdmi.h>
#include <asm/io.h>
int detect_hdmi(struct display_info_t const *dev)
{
	struct hdmi_regs *hdmi	= (struct hdmi_regs *)HDMI_ARB_BASE_ADDR;
	return readb(&hdmi->phy_stat0) & HDMI_DVI_STAT;
}
#endif
