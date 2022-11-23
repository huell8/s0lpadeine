CC=g++
RM=rm -f
CFLAGS=-Wall --std=c++17
TARGET=demo.cpp
OUT=output

all:
	mkdir -p target
	${CC} ${CFLAGS} src/${TARGET} -o target/${OUT}
clean:
	${RM} -r target
run:
	clear
	make all
	./target/${OUT}
