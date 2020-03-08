// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2020 FIXME
// Generated with linux-mdss-dsi-panel-driver-generator from vendor device tree:
//   Copyright (c) 2013, The Linux Foundation. All rights reserved. (FIXME)

#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/of.h>

#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>

struct sim {
	struct drm_panel panel;
	struct mipi_dsi_device *dsi;
	struct gpio_desc *reset_gpio;
	bool prepared;
};

static inline struct sim *to_sim(struct drm_panel *panel)
{
	return container_of(panel, struct sim, panel);
}

#define dsi_generic_write_seq(dsi, seq...) do {				\
		static const u8 d[] = { seq };				\
		int ret;						\
		ret = mipi_dsi_generic_write(dsi, d, ARRAY_SIZE(d));	\
		if (ret < 0)						\
			return ret;					\
	} while (0)

static void sim_reset(struct sim *ctx)
{
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	usleep_range(1000, 2000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	usleep_range(1000, 2000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	usleep_range(1000, 2000);
}

static int sim_on(struct sim *ctx)
{
	struct mipi_dsi_device *dsi = ctx->dsi;
	struct device *dev = &dsi->dev;
	int ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	dsi_generic_write_seq(dsi, 0x3c, 0x01, 0x06, 0x00, 0x05, 0x00);
	dsi_generic_write_seq(dsi, 0x14, 0x01, 0x04, 0x00, 0x00, 0x00);
	dsi_generic_write_seq(dsi, 0x64, 0x01, 0x04, 0x00, 0x00, 0x00);
	dsi_generic_write_seq(dsi, 0x68, 0x01, 0x04, 0x00, 0x00, 0x00);
	dsi_generic_write_seq(dsi, 0x6c, 0x01, 0x04, 0x00, 0x00, 0x00);
	dsi_generic_write_seq(dsi, 0x70, 0x01, 0x04, 0x00, 0x00, 0x00);
	dsi_generic_write_seq(dsi, 0x34, 0x01, 0x1f, 0x00, 0x00, 0x00);
	dsi_generic_write_seq(dsi, 0x10, 0x02, 0x1f, 0x00, 0x00, 0x00);
	dsi_generic_write_seq(dsi, 0x04, 0x01, 0x01, 0x00, 0x00, 0x00);
	dsi_generic_write_seq(dsi, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00);
	dsi_generic_write_seq(dsi, 0x50, 0x04, 0x20, 0x01, 0xf0, 0x03);
	dsi_generic_write_seq(dsi, 0x54, 0x04, 0x02, 0x00, 0x30, 0x00);
	dsi_generic_write_seq(dsi, 0x58, 0x04, 0x20, 0x03, 0x30, 0x00);
	dsi_generic_write_seq(dsi, 0x5c, 0x04, 0x02, 0x00, 0x40, 0x00);
	dsi_generic_write_seq(dsi, 0x60, 0x04, 0x00, 0x05, 0x20, 0x00);
	dsi_generic_write_seq(dsi, 0x64, 0x04, 0x01, 0x00, 0x00, 0x00);
	dsi_generic_write_seq(dsi, 0xa0, 0x04, 0x06, 0x80, 0x44, 0x00);
	usleep_range(2000, 3000);
	dsi_generic_write_seq(dsi, 0xa0, 0x04, 0x06, 0x80, 0x04, 0x00);
	usleep_range(2000, 3000);
	dsi_generic_write_seq(dsi, 0x04, 0x05, 0x04, 0x00, 0x00, 0x00);
	usleep_range(2000, 3000);
	dsi_generic_write_seq(dsi, 0x80, 0x04, 0x00, 0x01, 0x02, 0x03);
	dsi_generic_write_seq(dsi, 0x84, 0x04, 0x04, 0x07, 0x05, 0x08);
	dsi_generic_write_seq(dsi, 0x88, 0x04, 0x09, 0x0a, 0x0e, 0x0f);
	dsi_generic_write_seq(dsi, 0x8c, 0x04, 0x0b, 0x0c, 0x0d, 0x10);
	dsi_generic_write_seq(dsi, 0x90, 0x04, 0x16, 0x17, 0x11, 0x12);
	dsi_generic_write_seq(dsi, 0x94, 0x04, 0x13, 0x14, 0x15, 0x1b);
	dsi_generic_write_seq(dsi, 0x98, 0x04, 0x18, 0x19, 0x1a, 0x06);
	dsi_generic_write_seq(dsi, 0x9c, 0x04, 0x01, 0x00, 0x00, 0x00);

	return 0;
}

static int sim_off(struct sim *ctx)
{
	struct mipi_dsi_device *dsi = ctx->dsi;
	struct device *dev = &dsi->dev;
	int ret;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	dsi_generic_write_seq(dsi, 0xa0, 0x04, 0x01, 0x00, 0x00, 0x00);
	usleep_range(1000, 2000);
	dsi_generic_write_seq(dsi, 0x9c, 0x04, 0x00, 0x00, 0x00, 0x00);

	return 0;
}

static int sim_prepare(struct drm_panel *panel)
{
	struct sim *ctx = to_sim(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	if (ctx->prepared)
		return 0;

	sim_reset(ctx);

	ret = sim_on(ctx);
	if (ret < 0) {
		dev_err(dev, "Failed to initialize panel: %d\n", ret);
		gpiod_set_value_cansleep(ctx->reset_gpio, 0);
		return ret;
	}

	ctx->prepared = true;
	return 0;
}

static int sim_unprepare(struct drm_panel *panel)
{
	struct sim *ctx = to_sim(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	if (!ctx->prepared)
		return 0;

	ret = sim_off(ctx);
	if (ret < 0)
		dev_err(dev, "Failed to un-initialize panel: %d\n", ret);

	gpiod_set_value_cansleep(ctx->reset_gpio, 0);

	ctx->prepared = false;
	return 0;
}

static const struct drm_display_mode sim_mode = {
	.clock = (1280 + 52 + 4 + 48) * (800 + 32 + 6 + 64) * 60 / 1000,
	.hdisplay = 1280,
	.hsync_start = 1280 + 52,
	.hsync_end = 1280 + 52 + 4,
	.htotal = 1280 + 52 + 4 + 48,
	.vdisplay = 800,
	.vsync_start = 800 + 32,
	.vsync_end = 800 + 32 + 6,
	.vtotal = 800 + 32 + 6 + 64,
	.vrefresh = 60,
	.width_mm = 228,
	.height_mm = 149,
};

static int sim_get_modes(struct drm_panel *panel,
			 struct drm_connector *connector)
{
	struct drm_display_mode *mode;

	mode = drm_mode_duplicate(connector->dev, &sim_mode);
	if (!mode)
		return -ENOMEM;

	drm_mode_set_name(mode);

	mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
	connector->display_info.width_mm = mode->width_mm;
	connector->display_info.height_mm = mode->height_mm;
	drm_mode_probed_add(connector, mode);

	return 1;
}

static const struct drm_panel_funcs sim_panel_funcs = {
	.prepare = sim_prepare,
	.unprepare = sim_unprepare,
	.get_modes = sim_get_modes,
};

static int sim_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct sim *ctx;
	int ret;

	ctx = devm_kzalloc(dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_LOW);
	if (IS_ERR(ctx->reset_gpio)) {
		ret = PTR_ERR(ctx->reset_gpio);
		dev_err(dev, "Failed to get reset-gpios: %d\n", ret);
		return ret;
	}

	ctx->dsi = dsi;
	mipi_dsi_set_drvdata(dsi, ctx);

	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE |
			  MIPI_DSI_MODE_EOT_PACKET;

	drm_panel_init(&ctx->panel, dev, &sim_panel_funcs,
		       DRM_MODE_CONNECTOR_DSI);

	ret = drm_panel_of_backlight(&ctx->panel);
	if (ret) {
		dev_err(dev, "Failed to get backlight: %d\n", ret);
		return ret;
	}

	ret = drm_panel_add(&ctx->panel);
	if (ret < 0) {
		dev_err(dev, "Failed to add panel: %d\n", ret);
		return ret;
	}

	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to attach to DSI host: %d\n", ret);
		return ret;
	}

	return 0;
}

static int sim_remove(struct mipi_dsi_device *dsi)
{
	struct sim *ctx = mipi_dsi_get_drvdata(dsi);
	int ret;

	ret = mipi_dsi_detach(dsi);
	if (ret < 0)
		dev_err(&dsi->dev, "Failed to detach from DSI host: %d\n", ret);

	drm_panel_remove(&ctx->panel);

	return 0;
}

static const struct of_device_id sim_of_match[] = {
	{ .compatible = "sim" }, // FIXME
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, sim_of_match);

static struct mipi_dsi_driver sim_driver = {
	.probe = sim_probe,
	.remove = sim_remove,
	.driver = {
		.name = "panel-sim",
		.of_match_table = sim_of_match,
	},
};
module_mipi_dsi_driver(sim_driver);

MODULE_AUTHOR("linux-mdss-dsi-panel-driver-generator <fix@me>"); // FIXME
MODULE_DESCRIPTION("DRM driver for ss_dsi_panel_TC358764_LTL101A106_WXGA");
MODULE_LICENSE("GPL v2");
