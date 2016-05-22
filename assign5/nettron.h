#ifndef NETTRON_H
#define NETTRON_H

#include <stdint.h>

// Networking parameters
#define MULTITRON_DEFAULT_IP "127.0.0.1"
#define MULTITRON_DEFAULT_PORT 3110

// Opcodes for MultiTron hardware
#define MTRON_POWERON 0x00
#define MTRON_READ_LINE 0x01
#define MTRON_WRITE_LINE 0x02
#define MTRON_POWEROFF 0x1F

// Resolution of the individual displays
#define SUBDISPLAY_WIDTH 256
#define SUBDISPLAY_HEIGHT 128

// Function to implement
int tronctl_net(int *sock, uint32_t instr, void *data, int datalen);

#endif
