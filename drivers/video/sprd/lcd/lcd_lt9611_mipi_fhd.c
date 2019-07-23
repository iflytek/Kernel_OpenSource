#include "sprd_panel.h"
#include "sprd_dsi.h"
#include "dsi/mipi_dsi_api.h"
#include "sprd_dphy.h"
#include <i2c.h>

static int lt9611_init(void)
{
	struct sprd_dsi *dsi = &dsi_device;
	struct sprd_dphy *dphy = &dphy_device;

	pr_info("lt9611_power\n");
	mipi_dsi_lp_cmd_enable(dsi, true);
	mipi_dsi_set_work_mode(dsi, SPRD_MIPI_MODE_VIDEO);
	mipi_dsi_state_reset(dsi);
	mipi_dphy_hs_clk_en(dphy, true);

	return 0;
}

static int lt9611_readid(void)
{
	return 0;
}

static struct panel_ops lt9611_ops = {
	.init = lt9611_init,
	.read_id = lt9611_readid,
};

static struct panel_info lt9611_info = {
	/* common parameters */
	.lcd_name = "lcd_lt9611_mipi_fhd",
	.type = SPRD_PANEL_TYPE_MIPI,
	.bpp = 24,
//	.fps = 60,
        .width = 1920,
        .height = 1080,

        /* DPI specific parameters */
        .pixel_clk = 153600000,
        .rgb_timing = {
                .hfp = 32,
                .hbp = 32,
                .hsync = 4,
                .vfp = 176,
                .vbp = 16,
                .vsync = 10,
        },

        /* MIPI DSI specific parameters */
        .phy_freq = 1000000,
	.lane_num = 4,
	.work_mode = SPRD_MIPI_MODE_VIDEO,
	.burst_mode = PANEL_VIDEO_BURST_MODE,
	.nc_clk_en = false,
};

struct panel_driver lt9611_driver = {
	.info = &lt9611_info,
	.ops = &lt9611_ops,
};
