/*
 * lcd_def.c
 *
 *  Created on: Nov 12, 2014
 *      Author: Morgoth
 */

#include "lcd_def.h"

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

RASTER_TIMINGS lcd_TFT43AB_OMAP35x_beaglebone_exp =
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

RASTER_TIMINGS lcd_720p_50hz_beaglebone_exp =
{
/*X size*/		1280,
/*Y size*/		720,
/*ref clk*/		24000000,
/*pll M*/		124,
/*pll N*/		22,
/*pll M2*/		1,
/*raster clk*/	64500000,
/*palete len*/	25,
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
