
mce: mce.cpp mce.hpp main.cpp inputbuffer.cpp inputbuffer.hpp
	g++ -O3 -std=c++11 ./mce.cpp ./main.cpp ./inputbuffer.cpp -o mce.out
debug: mce.cpp mce.hpp main.cpp inputbuffer.cpp inputbuffer.hpp
	g++ -ggdb -O0 -std=c++11 ./mce.cpp ./main.cpp ./inputbuffer.cpp -o debug_mce.out
