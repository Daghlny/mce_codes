
mce: mce.cpp mce.hpp inputbuffer.cpp inputbuffer.hpp
	g++ -O3 -std=c++11 ./mce.cpp ./inputbuffer.cpp -o mce.out
debug: mce.cpp mce.hpp inputbuffer.cpp inputbuffer.hpp
	g++ -ggdb -O0 -std=c++11 ./mce.cpp ./inputbuffer.cpp -o debug_mce.out
bk: mce.hpp bkMain.cpp inputbuffer.cpp inputbuffer.hpp bk.cpp
	g++ -O3 -std=c++11 ./bkMain.cpp ./bk.cpp ./inputbuffer.cpp -o bk.out
bkdebug: mce.hpp bkMain.cpp inputbuffer.cpp inputbuffer.hpp bk.cpp
	g++ -ggdb -O0 -std=c++11 ./bkMain.cpp ./bk.cpp ./inputbuffer.cpp -o debug_bk.out

