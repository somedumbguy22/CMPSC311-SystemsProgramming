### Architecture choice

# Uncomment for 64-bit
LIBRARY = libmtnet32.a

# Uncomment for 32-bit
#LIBRARY = libmtnet32.a


### Variables

CFLAGS = -g -std=c99 -Wall


### File lists

BINS = server client
OBJS = nettron.o


### Phony targets all/clean

.PHONY: all clean

all: $(BINS)

clean:
	$(RM) $(BINS) $(OBJS)


### Build rules

# Be sure to link in the library
$(BINS): $(OBJS) $(LIBRARY)

# Update if the header file is changed
nettron.o: nettron.h
