.PHONY: all clean

SRC = $(wildcard *.c)
OBJ = $(subst .c,.o,$(SRC))
PROJECT = udpTest.exe
DYNAMICLIBS = udp_server
STATICLIBS = udpserver
DYNAMICFLAGS = -Bdynamic




$(PROJECT): $(OBJ)
	g++ -o $@ $^ -L. $(DYNAMICFLAGS) $(addprefix -l,$(DYNAMICLIBS))

%.o: %.c
	g++ -o $@ -c $<
