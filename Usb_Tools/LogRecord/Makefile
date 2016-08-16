CFLAGS += -O2  -D__UNIX__   -Wall -I./

LDFLAGS += -L./
LDLIBS += -lpthread 


CPPFILES=\
	Logger.cpp \
	main.cpp\


OBJFILES = $(CPPFILES:%.cpp=%.o)


all:LogRecord
LogRecord:$(OBJFILES) 
	g++ -O2 -D __UNIX__ -Wall -o LogRecord $(OBJFILES) $(LDFLAGS) $(LDLIBS)

$(OBJFILES):%.o:%.cpp
	g++ -g -c $(CFLAGS) -o $@ $<


clean:
	-rm -f *.o  LogRecord
