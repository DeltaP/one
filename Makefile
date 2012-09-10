MPICXX=mpicxx
CXX=g++
CXXFLAGS  = -g -Wall -std=c99 -Wextra -pedantic

INCLUDES =

all: parallel_simpson serial_simpson ring_hello

parallel_simpson: parallel_simpson.cpp
	${MPICXX} $(CXXFLAGS)   $^ -o $@  
	@printf 'Linked\n'


serial_simpson: serial_simpson.cpp
	${CXX} $(CXXFLAGS)  $^ -o $@  
	@printf 'Linked\n'

ring_hello: ring_hello.cpp
	${MPICXX} $(CXXFLAGS)   $^ -o $@  
	@printf 'Linked\n'

clean: 	
	rm -f *.o parallel_simpson

new: clean all

.PHONY: all new clean

