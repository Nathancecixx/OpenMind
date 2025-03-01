CXX		 = g++

EXE		 = CHANGEME

SRC		 = ../src
INCLUDE	 = ../include
LIB		 = ../lib

FLAGS	 = -std=c++20 -I$(INCLUDE) -L$(LIB)
CXXFLAGS = -c -Wall $(FLAGS)
LDFLAGS	 = $(FLAGS) -o

OBJECTS	 = main.o FpsTracker.o

ldsflkhskhlfl: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(EXE)

FpsTracker.o: $(SRC)/FpsTracker.cpp $(INCLUDE)/FpsTracker.h
	$(CXX) $(CXXFLAGS) $<

main.o: $(SRC)/main.cpp
	$(CXX) $(CXXFLAGS) $<

clean:
	del -q *.exe *.o
