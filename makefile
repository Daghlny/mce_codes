
HEADERS=./mce.hpp ./inputbuffer.hpp
INCLUDES=-I ./rapidjson/include/
CPPVERSION = c++11
GDBFLAGS= -O0 -std=$(CPPVERSION) -ggdb
GCCFLAGS= -O3 -std=$(CPPVERSION)
COMPILER= g++

asg: neighbourhood.cpp inputbuffer.cpp $(HEADERS)
	$(COMPILER) $(GCCFLAGS) $(INCLUDES) ./neighbourhood.cpp ./inputbuffer.cpp ./graph.cpp -o asg.out
mce: mce.cpp inputbuffer.cpp $(HEADERS)
	$(COMPILER) $(GCCFLAGS) ./mce.cpp ./inputbuffer.cpp -o mce.out
debug: neighbourhood.cpp inputbuffer.cpp $(HEADERS)
	$(COMPILER) $(GDBFLAGS) $(INCLUDES) ./neighbourhood.cpp ./inputbuffer.cpp -o debug_mce.out
bk: bkMain.cpp inputbuffer.cpp bk.cpp $(HEADERS)
	$(COMPILER) $(GCCFLAGS) ./bkMain.cpp ./bk.cpp ./inputbuffer.cpp -o bk.out
bkdebug: bkMain.cpp inputbuffer.cpp bk.cpp $(HEADERS)
	$(COMPILER) $(GDBFLAGS) ./bkMain.cpp ./bk.cpp ./inputbuffer.cpp -o debug_bk.out
mat: bitMatrix.cpp bitMatrix.hpp main.cpp
	$(COMPILER) $(GDBFLAGS) ./bitMatrix.cpp ./main.cpp -o debug_bitMat.out
