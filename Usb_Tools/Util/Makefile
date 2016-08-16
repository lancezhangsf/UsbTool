CFLAGS += -O2  -D__UNIX__   -Wall -I./

LDFLAGS += -L./
LDLIBS += -lpthread -ldl


CPPFILES=\
	./SerialFind.cpp\
    ./main.cpp\


OBJFILES = $(CPPFILES:%.cpp=%.o)


all:test
test:$(OBJFILES) 
	g++ -O2 -D __UNIX__ -Wall -o test $(OBJFILES) $(LDFLAGS) $(LDLIBS)

$(OBJFILES):%.o:%.cpp
	g++ -g -c $(CFLAGS) -o $@ $<


clean:
	rm -f *.o  test
