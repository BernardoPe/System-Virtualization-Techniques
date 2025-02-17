.PHONY: all clean

all: prog prog2.so

prog:
	gcc -o prog prog.c -pedantic -Wall 

prog2.so:
	gcc -shared -o prog2.so prog2.c -fPIC -pedantic -Wall

clean:
	rm -f prog.o prog prog2.so

