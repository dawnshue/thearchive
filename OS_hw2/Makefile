CC = g++
CXX = g++

INCLUDES = 

CFLAGS = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS =
LDLIBS =

executables = OSlab2
objects = OSlab2.o #scheduler.o FCFS.o LCFS.o RR.o SJF.o

.PHONY: default
default: $(executables)

$(executables):
#scheduler.o FCFS.o LCFS.o RR.o SJF.o
$(objects): process.h scheduler.h
#scheduler.h FCFS.h LCFS.h RR.h SJF.h

.PHONY: clean
clean:
	rm -f *~ a.out core $(objects) $(executables)

.PHONY: all
all: clean default
