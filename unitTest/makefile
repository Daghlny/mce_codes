
TEST_DIR = .
USER_DIR = ..
GTEST_DIR = ../googletest/googletest
CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem ../
CXXFLAGS += -g -Wall -Wextra -pthread -std=c++11
TESTS = bitMatrix

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
				$(GTEST_DIR)/include/gtest/internal/*.h

all : $(TESTS)

clean:
	rm -f $(TEST) gtest.a gtest_main.a *.o

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

bitMatrix.o : $(USER_DIR)/bitMatrix.cpp $(USER_DIR)/bitMatrix.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/bitMatrix.cpp

bitMatrix_test.o : $(TEST_DIR)/bitMatrix_test.cpp $(TEST_DIR)/test.hpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/bitMatrix_test.cpp

bitMatrix : bitMatrix_test.o gtest_main.a bitMatrix.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

