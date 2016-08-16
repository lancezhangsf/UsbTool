CFLAGS += -O2  -D__UNIX__   -Wall -I./

LDFLAGS += -L./
LDLIBS += -lpthread -ldl


CPPFILES=\
	../LogRecord/Logger.cpp\
	SerialPort.cpp\
	main.cpp\



OBJFILES = $(CPPFILES:%.cpp=%.o)


all:SerialPort
SerialPort:$(OBJFILES) 
	g++ -O2 -D __UNIX__ -Wall -o SerialPort $(OBJFILES) $(LDFLAGS) $(LDLIBS)

$(OBJFILES):%.o:%.cpp
	g++ -g -c $(CFLAGS) -o $@ $<


clean:
	-rm -f *.o  SerialPort
