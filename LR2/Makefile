CC = g++
CFLAGS =

all: main

main:
	$(CC) $(CFLAGS) -o main main.cpp B-tree.cpp

run:
	./main

clean:
	rm main

format:
	clang-format -i *.cpp
