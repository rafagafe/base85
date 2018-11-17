
CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

src = $(wildcard *.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d) 

build: example-01.exe example-02.exe

clean:
	rm -rf *.d
	rm -rf *.o
	rm -rf *.exe
	
all: clean build

test: test.exe
	./test.exe

example-01.exe: example-01.o base85.o
	gcc $(CFLAGS) -o $@ $^

example-02.exe: example-02.o base85.o
	gcc $(CFLAGS) -o $@ $^

test.exe: test.o base85.o
	gcc $(CFLAGS) -o $@ $^

-include $(dep)

%.d: %.c
	$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@
