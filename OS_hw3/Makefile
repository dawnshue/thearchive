CC = g++
CXX = g++

INCLUDES = 

CFLAGS = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS =
LDLIBS =

executables = mmu
objects = mmu.o

.PHONY: default
default: $(executables)

$(executables):
#scheduler.o FCFS.o
$(objects): Frame.h VSpace.h Replacer.h

.PHONY: clean
clean:
	rm -f *~ a.out core $(objects) $(executables)

.PHONY: exec
exec:
	./mmu -al -f8 -opfOPFS inputs/in60 inputs/rfile

.PHONY: all
all: clean default
