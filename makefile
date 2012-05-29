SOURCES = $(wildcard src/*.c)

UNAME := $(shell uname)
PWD   := $(shell pwd)

TARGET = tinyga
CC = gcc
CFLAGS = -v -g -Wall

all: 
	${CC} ${CFLAGS} -o ${TARGET} ${SOURCES} ${INCLUDES} ${LIBS} 

clean:
	rm -f ${TARGET}