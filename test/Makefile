.PHONY: all clean

SRC = $(wildcard *.c)
DEPS = macros.h
OBJ = $(subst .c,.o,$(SRC))
CFLAGS=-I.
PROJECT = udpTest.exe
DYNAMICLIBS = udp_server
STATICLIBS = udpserver
DYNAMICFLAGS = -Bdynamic




$(PROJECT): $(OBJ)
	gcc -o $@ $^ -L. $(DYNAMICFLAGS) $(addprefix -l,$(DYNAMICLIBS))

%.o: %.c $(DEPS)
	gcc -c -o $@ $< $(CFLAGS)
	
