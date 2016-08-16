###########################################
# Simple Makefile for HIDAPI test program
#
# Alan Ott
# Signal 11 Software
# 2010-06-01
###########################################

all: hidtest

CC       ?= gcc
CFLAGS   ?= -Wall -g

CXX      ?= g++
CXXFLAGS ?= -Wall -g

COBJS     = hid-libusb.o
CPPOBJS   = main.o hid_interface.o
OBJS      = $(COBJS) $(CPPOBJS)
LIBS      = -L/usr/local/lib -lusb-1.0
INCLUDES ?= -I/usr/local/include/libusb-1.0


hidtest: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o hidtest

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@

$(CPPOBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) $< -o $@

clean:
	rm -f $(OBJS) hidtest

.PHONY: clean
