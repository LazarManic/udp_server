# Makefile cheat sheet
#
# Automatic variables:
# $@ - filename of target of rule
# $% - target member name, when target is archive member
# $< - name of first prerequisite
# $? - names of all prerequisites newer than targer
# $^ - names of all prerequisites

SRCDIR = source/
INCDIR = includes/
OBJDIR = $(SRCDIR)obj/

SRC = $(wildcard $(SRCDIR)*.cpp)
INC = $(wildcard $(INCDIR)*.hpp)
OBJ = $(patsubst $(SRCDIR)%.cpp, $(OBJDIR)%.o, $(SRC))
##OBJ = $(subst .cpp,.o,$(SRC))


ifeq ($(OS),Windows_NT)
	PROJECT = udp_server.exe
else
	PROJECT = udp_server
endif

ifeq ($(OS),Windows_NT)
	PROJECTLIB = udp_server.dll
else
	PROJECTLIB = udp_server.so
endif

ifeq ($(OS),Windows_NT)
	STATICLIB = udpserver.lib
else
	STATICLIB = libudpserver.a
endif

CXX := g++
AR := ar
ifeq ($(OS),Windows_NT)
	CXXFLAGS := -std=c++11 -g -O2 -Wall -mthreads -Iinclude \
		-I../../boost_1_34_1 -D_WIN32_WINNT=0x0501 -DASIO_STANDALONE
	LDFLAGS = -g -O2 -mthreads -lws2_32 -lmswsock
else
	CXXFLAGS := -std=c++11 -g -O2 -Wall -pthread -Iinclude \
		-I../../boost_1_34_1 -DASIO_STANDALONE
	LDFLAGS = -g -O2 -pthread
endif

all: $(PROJECT)

full: executable dynamic static copy

executable: $(PROJECT)

dynamic: $(PROJECTLIB)

static: $(STATICLIB)

$(PROJECT): $(OBJ) 
	$(CXX) -o $@ $^ $(LDFLAGS)

$(PROJECTLIB): $(OBJ)
	$(CXX) -shared -o $@ $^ $(LDFLAGS) 

$(STATICLIB): $(OBJ)
	$(AR) rcs $@ $^

$(OBJDIR)%.o: $(SRCDIR)%.cpp 
	$(CXX) -o $@ -c $(CXXFLAGS) $<

copy:
ifeq ($(OS),Windows_NT)
	copy udp_server.dll test\udp_server.dll /Y
	copy source\udp_server.h test\udp_server.h /Y
	copy udp_server.dll C:\Users\arijana\Documents\coinesApp\COINES\v1.2\coinesAPI\udp_server.dll /Y
	copy source\udp_server.h C:\Users\arijana\Documents\coinesApp\COINES\v1.2\coinesAPI\udp_server.h /Y
	copy udp_server.dll C:\Users\arijana\Documents\coinesApp\COINES\v1.2\examples\c\bmi08x\read_sensor_data\udp_server.dll /Y
	copy source\udp_server.h C:\Users\arijana\Documents\coinesApp\COINES\v1.2\examples\c\bmi08x\read_sensor_data\udp_server.h /Y
else
	cp udp_server test\udp_server
	cp source\udp_server.h test\udp_server.h
	cp udp_server C:\Users\arijana\Documents\coinesApp\COINES\v1.2\coinesAPI\udp_server
	cp source\udp_server.h C:\Users\arijana\Documents\coinesApp\COINES\v1.2\coinesAPI\udp_server.h
	cp udp_server C:\Users\arijana\Documents\coinesApp\COINES\v1.2\examples\c\bmi08x\read_sensor_data\udp_server
	cp source\udp_server.h C:\Users\arijana\Documents\coinesApp\COINES\v1.2\examples\c\bmi08x\read_sensor_data\udp_server.h
endif

clean:
ifeq ($(OS),Windows_NT)
	del $(subst /,\, $(PROJECT) $(PROJECTLIB) $(OBJ))
else
	rm -rf $(PROJECT) $(PROJECTLIB) $(OBJ)
endif

.PHONY: clean copy all executable dynamic static full
