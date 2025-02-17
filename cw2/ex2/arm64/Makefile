.PHONY: all clean

all: prog

prog: prog.o
	aarch64-linux-gnu-ld -o prog prog.o

prog.o: prog.s
	aarch64-linux-gnu-as -o prog.o prog.s

clean:
	rm -f prog.o prog

