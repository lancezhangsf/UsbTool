CFLAGS += -O2  -D__UNIX__   -Wall -I./

LDFLAGS += -L./
LDLIBS += -lpthread -ldl


CPPFILES=\
	tinystr.cpp \
	tinyxml.cpp\
	tinyxmlerror.cpp\
	tinyxmlparser.cpp\
	main.cpp\



OBJFILES = $(CPPFILES:%.cpp=%.o)


all:XmlTest
XmlTest:$(OBJFILES) 
	g++ -O2 -D __UNIX__ -Wall -o XmlTest $(OBJFILES) $(LDFLAGS) $(LDLIBS)

$(OBJFILES):%.o:%.cpp
	g++ -g -c $(CFLAGS) -o $@ $<


clean:
	-rm -f *.o  XmlTest
