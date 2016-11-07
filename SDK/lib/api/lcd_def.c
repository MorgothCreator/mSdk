/*
 * lcd_def.c
 *
 *  Created on: Nov 12, 2014
 *      Author: Morgoth
 */

#include "lcd_def.h"

RASTER_TIMINGS lcd_OTM8009A =
{
/*X size*/		480,
/*Y size*/		800,
/*ref clk*/		0,
/*pll M*/		0,
/*pll N*/		125,
/*pll M2*/		0,
/*raster clk*/	27429000,
/*palete len*/	0,
/*bus size*/	0,
/*hsw*/			120,
/*hfp*/			120,
/*hbp*/			120,
/*vsw*/			12,
/*vfp*/			12,
/*vbp*/			12
};

RASTER_TIMINGS lcd_MI0283 =
{
/*X size*/		240,
/*Y size*/		320,
/*ref clk*/		0,
/*pll M*/		0,
/*pll N*/		0,
/*pll M2*/		0,
/*raster clk*/	0,
/*palete len*/	0,
/*bus size*/	8,
/*hsw*/			0,
/*hfp*/			0,
/*hbp*/			0,
/*vsw*/			0,
/*vfp*/			0,
/*vbp*/			0
};

RASTER_TIMINGS lcd_S035Q01_beaglebone_exp =
{
/*X size*/		320,
/*Y size*/		240,
/*ref clk*/		24000000,
/*pll M*/		64,
/*pll N*/		23,
/*pll M2*/		1,
/*raster clk*/	6888600,
/*palete len*/	8,
/*bus size*/	16,
/*hsw*/			48,
/*hfp*/			40,
/*hbp*/			22,
/*vsw*/			3,
/*vfp*/			13,
/*vbp*/			11
};

RASTER_TIMINGS lcd_TFT43AB_OMAP35x_devkit8600_exp =
{
/*X size*/		480,
/*Y size*/		272,
/*ref clk*/		24000000,
/*pll M*/		64,
/*pll N*/		23,
/*pll M2*/		1,
/*raster clk*/	14901480,
/*palete len*/	8,
/*bus size*/	16,
/*hsw*/			40,
/*hfp*/			255,
/*hbp*/			6,
/*vsw*/			9,
/*vfp*/			22,
/*vbp*/			15
};

RASTER_TIMINGS lcd_TFT43_TMDSSK3358 =
{
/*X size*/		480,
/*Y size*/		272,
/*ref clk*/		24000000,
/*pll M*/		64,
/*pll N*/		23,
/*pll M2*/		1,
/*raster clk*/	14901480,
/*palete len*/	8,
/*bus size*/	24,
/*hsw*/			40,
/*hfp*/			255,
/*hbp*/			6,
/*vsw*/			9,
/*vfp*/			22,
/*vbp*/			15
};

RASTER_TIMINGS lcd_AT070TN92_beaglebone_exp =
{
/*X size*/		800,
/*Y size*/		480,
/*ref clk*/		24000000,
/*pll M*/		64,
/*pll N*/		23,
/*pll M2*/		1,
/*raster clk*/	31996800,
/*palete len*/	8,
/*bus size*/	24,
/*hsw*/			80,
/*hfp*/			16,
/*hbp*/			160,
/*vsw*/			3,
/*vfp*/			1,
/*vbp*/			21
};

RASTER_TIMINGS lcd_720p_480_60hz_beaglebone_exp =
{
/*X size*/		720,
/*Y size*/		480,
/*ref clk*/		24000000,
/*pll M*/		54,
/*pll N*/		23,
/*pll M2*/		1,
/*raster clk*/	27000000,
/*palete len*/	25,
/*bus size*/	24,
/*hsw*/			62,
/*hfp*/			60,
/*hbp*/			16,
/*vsw*/			6,
/*vfp*/			30,
/*vbp*/			9
};

RASTER_TIMINGS lcd_720p_50hz_beaglebone_exp =
{
/*X size*/		1280,
/*Y size*/		720,
/*ref clk*/		24000000,
/*pll M*/		124,
/*pll N*/		22,
/*pll M2*/		1,
/*raster clk*/	64500000,
/*palete len*/	26,
/*bus size*/	24,
/*hsw*/			40,
/*hfp*/			220,
/*hbp*/			440,
/*vsw*/			6,
/*vfp*/			4,
/*vbp*/			20
};

RASTER_TIMINGS lcd_1080p_24hz_beaglebone_exp =
{
/*X size*/		1920,
/*Y size*/		1080,
/*ref clk*/		24000000,
/*pll M*/		116,
/*pll N*/		22,
/*pll M2*/		1,
/*raster clk*/	60500000,
/*palete len*/	25,
/*bus size*/	24,
/*hsw*/			44,
/*hfp*/			148,
/*hbp*/			638,
/*vsw*/			5,
/*vfp*/			4,
/*vbp*/			36
};
