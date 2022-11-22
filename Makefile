CC=gcc
RM=rm -f
CFLAGS=-Wall --std=c17
TARGET=demo.c

all:
	mkdir -p target
	${CC} ${CFLAGS} src/${TARGET} -o target/${TARGET}
clean:
	${RM} -r target
run:
	clear
	make all
	./target/${TARGET}
