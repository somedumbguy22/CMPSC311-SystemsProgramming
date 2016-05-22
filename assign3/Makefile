### Architecture choice

# Uncomment for 64-bit
LIBRARY = libmtron32.a

# Uncomment for 32-bit
#LIBRARY = libmtron32.a


### File lists

BINS = simulate
OBJS = driver.o


### Phony targets all/clean

.PHONY: all clean

all: $(BINS)

clean:
	$(RM) $(BINS) $(OBJS)


### Build rules

# Be sure to link in the library
$(BINS): $(OBJS) $(LIBRARY)

# Update if the header file is changed
driver.o: driver.h mtron.h

simulate: mtron.h
