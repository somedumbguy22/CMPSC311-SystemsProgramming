#ifndef TRONCTL_H
#define TRONCTL_H

/*
 * External API for the MultiTron hardware.  Implementation is provided in the
 * libmtron.a library.
 */

#include <stdint.h>

// Opcodes for MultiTron hardware
#define MTRON_POWERON 0x00
#define MTRON_READ_LINE 0x01
#define MTRON_WRITE_LINE 0x02
#define MTRON_POWEROFF 0x1F

// Resolution of the individual displays
#define SUBDISPLAY_WIDTH 256
#define SUBDISPLAY_HEIGHT 128

// Function for communicating with hardware (see assignment document for usage)
int tronctl(uint32_t op, void *data);

// Function needed to run simulation (you don't need to use this yourself)
int simulate(int argc, char **argv);

#endif
