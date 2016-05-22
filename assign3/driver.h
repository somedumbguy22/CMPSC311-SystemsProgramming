#ifndef DRIVER_H
#define DRIVER_H

/*
 * Header file for the MultiTron driver.  This file defines the types and
 * functions needed to implement a driver for the MultiTron hardware.
 *
 * DO NOT put function implementations in this file, and DO NOT change the
 * function prototypes below!  The only thing you should need to change here is
 * to fill out the struct definition below.
 */

struct multitron {
	// Store any needed driver state in this struct.  If you use global
	// variables to keep track of state, the driver will stop working as
	// soon as you connect a second device... so don't do that!  Instead,
	// this struct will be passed to each of the driver functions so you can
	// read your state variables that way.

	uint8_t colorDepth;
	uint8_t maxDisplayRows;
	uint8_t maxDisplayColumns;
	uint32_t maxRowNumber;
	uint32_t maxColumnNumber;
	uint32_t currentColumn;
	uint32_t currentRow;
	uint32_t op;
	uint32_t display;
};

int mtron_init(struct multitron *mtron);
void mtron_destroy(struct multitron *mtron);

int mtron_getpixel(struct multitron *mtron, uint32_t x, uint32_t y,
		uint8_t *color);
int mtron_putpixel(struct multitron *mtron, uint32_t x, uint32_t y,
		uint8_t color);

int mtron_getrect(struct multitron *mtron, uint32_t x, uint32_t y,
		uint32_t w, uint32_t h, uint8_t *data);
int mtron_putrect(struct multitron *mtron, uint32_t x, uint32_t y,
		uint32_t w, uint32_t h, uint8_t *data);

#endif
