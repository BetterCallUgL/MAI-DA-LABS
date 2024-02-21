aLl : build
	./main < input.txt


v: build
	valgrind ./main < input.txt

build:
	g++ main.cpp -o main

c:
	rm main log.txt

gprof:
	g++ -pg -o main main.cpp
	./main < input.txt > output.txt
	gprof main gmon.out > log.txt