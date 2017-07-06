

build: example-01.exe example-02.exe

clean:
	rm -rf *.o
	rm -rf *.exe

all: clean build

test: test.exe
	./test.exe

example-01.exe: example-01.o base85.o
	gcc -o example-01.exe example-01.o base85.o

example-02.exe: example-02.o base85.o
	gcc -o example-02.exe example-02.o base85.o

test.exe: test.o base85.o
	gcc -o test.exe test.o base85.o

base85.o: base85.c base85.h
	gcc -c base85.c

example-01.o: example-01.c base85.h
	gcc -c example-01.c

example-02.o: example-02.c base85.h
	gcc -c example-02.c

test.o: test.c base85.h
	gcc -c test.c
