CC=gcc
CFLAGS=-Wall -Wextra -std=c99

#pick up all sources
SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)

#define targets
TARGETS=image_editor

build: $(OBJS) $(TARGETS)

image_editor:
	$(CC) $(CFLAGS) *.c -o image_editor -lm 

pack:
	zip -FSr 314CA_MaciucaAlexandruPetru_Tema3.zip README Makefile *.c *.h

run: build
	./image_editor
	

clean:
	rm -f $(TARGETS) $(OBJS)

.PHONY: pack clean
