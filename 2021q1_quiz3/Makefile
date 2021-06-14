all: space

space: spaceTest.cpp xs.h
	g++ -std=c++11 -fpermissive spaceTest.cpp

valgrind:
	valgrind --tool=massif --stacks=yes ./a.out

.PHONY: clean

clean:
	rm -rf *.out massif.out.*
