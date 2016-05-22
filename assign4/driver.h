#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>

/*
 * Header file for the MultiTron driver.  This file defines the types and
 * functions needed to implement a driver for the MultiTron hardware.
 */

struct multitron {
	// Number of rows and columns of subdisplays
	uint8_t rows, cols;
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

void verify(void);

#endif
