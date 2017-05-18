include = -Iinclude
SRC = position_reader.cpp overflow_time.cpp matrix.cpp solver.cpp path.cpp
MAIN_ENTRY = main.cpp

SRC_TEST = main.cpp

libs =  
libs_test = -lgtest -lpthread

main = main

sources = $(SRC:%=src/%) $(MAIN_ENTRY:%=src/%)
objs = $(SRC:%.cpp=objs/%.o) $(MAIN_ENTRY:%.cpp=objs/%.o)
objs_tests = $(SRC:%.cpp=objs/%.o) $(SRC_TEST:%.cpp=objs_tests/%.o)
depends = $(SRC:%.cpp=objs/%.cpp.d)  	\
	$(MAIN_ENTRY:%.cpp=objs/%.cpp.d) \
	$(SRC_TEST:%.cpp=objs_tests/%.cpp.d)

CC = g++ -std=c++11 -O3

all: $(main)

$(main): $(objs)
	$(CC) $^ -o $@ $(libs)

test: $(objs_tests)
	$(CC) $^ -o $@ $(libs) $(libs_test)

-include $(depends)

objs/%.cpp.d: src/%.cpp
	@mkdir -p $(@D)
	@$(CC) $(include) -MM $< -MT $(<:src/%.cpp=objs/%.o) -MF $@

objs_tests/%.cpp.d: tests/%.cpp
	@mkdir -p $(@D)
	@$(CC) $(include) -MM $< -MT $(<:tests/%.cpp=objs_tests/%.o) -MF $@

objs/%.o: src/%.cpp
	$(CC) $(include) -c $< -o $@

objs_tests/%.o: tests/%.cpp
	$(CC) $(include) -c $< -o $@

clean: 
	rm -rf objs/*
	rm -rf objs_tests/*
	rm $(main)
	rm test

.PHONY: all clean
