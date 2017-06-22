
USER_DIR = .
CPPFLAGS += -isystem ./
#CXXFLAGS += -ggdb -O0 -Wextra -pthread -std=c++11 
#CXXFLAGS += -O3 -Wextra -pthread -std=c++11 -fopenmp -Wno-unused-result -Wno-format
CXXFLAGS += -O0 -ggdb -Wextra -pthread -std=c++11 -fopenmp -Wno-unused-result -Wno-format
CXX = g++

HEADERS = $(USER_DIR)/mce.hpp \
		  $(USER_DIR)/bitMatrix.hpp \
		  $(USER_DIR)/Neighborhood.hpp \

AllObjects : graph.o bitMatrix.o Neighborhood.o inputbuffer.o BMBK.o main.o

graph.o : $(USER_DIR)/mce.hpp $(USER_DIR)/graph.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/graph.cpp

bitMatrix.o : $(USER_DIR)/mce.hpp $(USER_DIR)/bitMatrix.cpp $(USER_DIR)/bitMatrix.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/bitMatrix.cpp

Neighborhood.o : $(USER_DIR)/Neighborhood.cpp $(USER_DIR)/Neighborhood.hpp $(USER_DIR)/mce.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/Neighborhood.cpp

inputbuffer.o : $(USER_DIR)/inputbuffer.cpp $(USER_DIR)/inputbuffer.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/inputbuffer.cpp

BMBK.o : $(USER_DIR)/BMBK.cpp $(USER_DIR)/BMBK.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/BMBK.cpp

main.o : $(USER_DIR)/main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/main.cpp

main.out : graph.o bitMatrix.o Neighborhood.o inputbuffer.o BMBK.o main.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

clean : 
	rm -rf ./*.o ./*.a

all : $(AllObjects) main.out
	#rm -rf ./*.o ./*.a

HEADERS=./mce.hpp ./inputbuffer.hpp
INCLUDES=-I ./rapidjson/include/
CPPVERSION = c++11
GDBFLAGS= -O0 -std=$(CPPVERSION) -ggdb
GCCFLAGS= -O3 -std=$(CPPVERSION)
COMPILER= g++

asg: neighbourhood.cpp inputbuffer.cpp $(HEADERS)
	$(COMPILER) $(GCCFLAGS) $(INCLUDES) ./neighbourhood.cpp ./inputbuffer.cpp ./graph.cpp -o asg.out
debug: neighbourhood.cpp inputbuffer.cpp $(HEADERS)
	$(COMPILER) $(GDBFLAGS) $(INCLUDES) ./neighbourhood.cpp ./inputbuffer.cpp -o debug_mce.out
bk: bkMain.cpp inputbuffer.cpp bk.cpp $(HEADERS)
	$(COMPILER) $(GCCFLAGS) ./bkMain.cpp ./bk.cpp ./inputbuffer.cpp -o bk.out
bkdebug: bkMain.cpp inputbuffer.cpp bk.cpp $(HEADERS)
	$(COMPILER) $(GDBFLAGS) ./bkMain.cpp ./bk.cpp ./inputbuffer.cpp -o debug_bk.out
mat: bitMatrix.cpp bitMatrix.hpp main.cpp
	$(COMPILER) $(GDBFLAGS) ./bitMatrix.cpp ./main.cpp -o debug_bitMat.out


