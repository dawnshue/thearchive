CC = g++
CXX = g++

INCLUDES = 

CFLAGS = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS =
LDLIBS =

executables = iosched
objects = iosched.o

.PHONY: default
default: $(executables)

$(executables):
#scheduler.o FCFS.o
$(objects): Scheduler.h

.PHONY: clean
clean:
	rm -f *~ a.out core $(objects) $(executables)

.PHONY: exec
exec:
	./iosched -sf TestData/input0

.PHONY: all
all: clean default
