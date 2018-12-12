
all: bank-test

clean:
	rm -f bank-test

bank-test: bank-test.c bank.c bank.h
	gcc -g --std=c11 -o bank-test bank-test.c bank.c -lpthread

test: bank-test
	./bank-test

.PHONY: all clean test