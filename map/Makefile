
all: map-test

clean:
	rm -f map-test

map-test: map-test.c map.c map.h
	gcc -g --std=c11 -o map-test map-test.c map.c -lpthread

test: map-test
	./map-test

.PHONY: all clean test
