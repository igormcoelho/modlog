
ROOT_PATH=..
INC_PATH=../include

all: test

test: build/all_ut_test
	@echo "Executing tests"
	./build/all_ut_test

build/all_ut_test: all_ut.cpp
	mkdir -p build/
	g++ -g -O3 -Wfatal-errors -std=c++20 -pedantic -fsanitize=address -I$(INC_PATH) -Ithirdparty $<  -o $@   


# cleaning tests
clean:
	rm -f *.test
	rm -f *_test
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -rf build/
