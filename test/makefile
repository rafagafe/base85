
CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

src  = $(wildcard *.c)
src += $(wildcard ../*.c)
obj  = $(src:.c=.o)
dep  = $(obj:.o=.d) 

.PHONY: build all clean test

build: base85-test

clean:
	rm -rf $(dep)
	rm -rf $(obj)
	rm -rf base85-test
	
all: clean build

test: base85-test
	./base85-test

base85-test: $(obj)
	gcc $(CFLAGS) -o $@ $^

-include $(dep)

%.d: %.c
	$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@
