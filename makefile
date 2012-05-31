TINY := src/tiny.c
TINYTINY := src/tinytiny.c

UNAME := $(shell uname)
PWD   := $(shell pwd)

TARGET = tinyga
CC = gcc
CFLAGS = -v -g -Wall

all: tiny tinytiny

tiny:
	${CC} ${CFLAGS} -o ${TARGET} ${TINY} ${INCLUDES} ${LIBS} 

tinytiny:
	${CC} ${CFLAGS} -o tiny${TARGET} ${TINYTINY} ${INCLUDES} ${LIBS}

clean:
	rm -f ${TARGET} tiny${TARGET} 