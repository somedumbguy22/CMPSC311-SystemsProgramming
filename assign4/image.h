#ifndef IMAGE_H
#define IMAGE_H

// DO NOT CHANGE anything in this file!  It defines the API that you are
// implementing.

#include <stdint.h>

// This is called a forward declaration - it means that `struct multitron'
// exists but is defined elsewhere.
struct multitron;

// Magic number for the 3II image format
#define MAGIC_NUMBER_SIZE 4
static const uint8_t MAGIC_NUMBER[MAGIC_NUMBER_SIZE] = {'C', 'S', 0x03, 0x11};

// Feature flags for 3II
#define FEATURE_PLANES    (1 << 0)
#define FEATURE_INTERLACE (1 << 1)
#define FEATURE_MASK      (1 << 2)
#define FEATURE_HIRES     (1 << 3)

// Internal structure for images
struct image {
	// Width and height of the image
	int w, h;

	// Pointer to pixel data
	uint8_t *data;

	// Pointer to mask values
	uint8_t *mask;

	// Pointer to comment string
	char *comment;
};


int image_init(struct image *img, const char *fname);
void image_destroy(struct image *img);

int image_save(const struct image *img, const char *fname);

int image_draw(const struct image *img, struct multitron *mtron, int x, int y);
int image_get(struct image *img, struct multitron *mtron, int x, int y);

#endif
