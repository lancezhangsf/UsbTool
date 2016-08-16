	
all: Usb_Tool

CC       ?= gcc
CFLAGS   ?= -Wall -g

CXX      ?= g++
CXXFLAGS ?= -Wall -g

COBJS     = \
	./HidLib/hid-libusb.o\
	
CPPOBJS   = \
	./Util/SerialFind.o\
	./Util/UsbFind.o\
	./LogRecord/Logger.o\
	./HidLib/hid_interfaceimp.o\
	./SerialPort/SerialPort.o\
	./XmlParse/tinystr.o\
	./XmlParse/tinyxml.o\
	./XmlParse/tinyxmlerror.o\
	./XmlParse/tinyxmlparser.o\
    ./main.o\

OBJS      = $(COBJS) $(CPPOBJS)
LIBS      = -lusb-1.0 -lpthread
INCLUDES ?= -I/usr/local/include/libusb-1.0


Usb_Tool: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o Usb_Tool

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@

$(CPPOBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) $< -o $@

clean:
	rm -rf $(OBJS) *.o Usb_Tool *log

.PHONY: clean