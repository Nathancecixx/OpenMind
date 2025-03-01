CXX		 = g++

EXE		 = Conhacks

SRC		 = ../src
INCLUDE	 = ../include
LIB		 = ../lib

FLAGS	 = -std=c++23 -I$(INCLUDE) -L$(LIB) -lraylib -lgdi32 -lwinmm
CXXFLAGS = -c -Wall $(FLAGS)
LDFLAGS	 = $(FLAGS) -o

OBJECTS	 = main.o

all: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(EXE)

main.o: $(SRC)/main.cpp
	$(CXX) $(CXXFLAGS) $<

clean:
	del -q *.exe *.o
