### Architecture choice

# Uncomment for 64-bit
LIBRARY = libmtdriver32.a

# Uncomment for 32-bit
#LIBRARY = libmtdriver32.a


### Variables

CFLAGS = -g -std=c99 -Wall


### File lists

BINS = verify
OBJS = image.o


### Phony targets all/clean

.PHONY: all clean

all: $(BINS)

clean:
	$(RM) $(BINS) $(OBJS)


### Build rules

# Be sure to link in the library
$(BINS): $(OBJS) $(LIBRARY)

image.o: image.h
