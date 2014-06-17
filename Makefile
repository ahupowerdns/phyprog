-include sysdeps/$(shell uname).inc

VERSION=0.1
CXXFLAGS?=-ggdb -Iahutils -I. -I/usr/include/eigen3 -Wall -O3 -MMD -MP  $(CXX2011FLAGS) # -Wno-unused-local-typedefs 
LDFLAGS=$(CXX2011FLAGS)   

PROGRAMS=gendata

all: $(PROGRAMS)

gendata: gendata.o ahutils/rfile.o
	g++ $< -o $@

clean:
	-rm  -f *.d *.o *~ testrunner


-include *.d