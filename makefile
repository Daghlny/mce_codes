
HEADERS=./mce.hpp ./inputbuffer.hpp
CPPVERSION = c++11
GDBFLAGS= -O0 -std=$(CPPVERSION) -ggdb
GCCFLAGS= -O3 -std=$(CPPVERSION)
COMPILER= g++

asg: neighbourhood.cpp inputbuffer.cpp $(HEADERS)
	$(COMPILER) $(GCCFLAGS) ./neighbourhood.cpp ./inputbuffer.cpp -o asg.out
mce: mce.cpp inputbuffer.cpp $(HEADERS)
	$(COMPILER) $(GCCFLAGS) ./mce.cpp ./inputbuffer.cpp -o mce.out
debug: mce.cpp inputbuffer.cpp $(HEADERS)
	$(COMPILER) $(GDBFLAGS) ./mce.cpp ./inputbuffer.cpp -o debug_mce.out
bk: bkMain.cpp inputbuffer.cpp bk.cpp $(HEADERS)
	$(COMPILER) $(GCCFLAGS) ./bkMain.cpp ./bk.cpp ./inputbuffer.cpp -o bk.out
bkdebug: bkMain.cpp inputbuffer.cpp bk.cpp $(HEADERS)
	$(COMPILER) $(GDBFLAGS) ./bkMain.cpp ./bk.cpp ./inputbuffer.cpp -o debug_bk.out

