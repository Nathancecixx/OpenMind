CXX		 = g++

EXE		 = Conhacks

SRC		 = ../src
INCLUDE	 = ../include
LIB		 = ../lib
SHARED	 = ../../Shared

FLAGS	 = -std=c++23 -I$(INCLUDE) -I$(SHARED) -L$(LIB) -lraylib -lgdi32 -lwinmm -lws2_32 -mwindows
CXXFLAGS = -c -Wall $(FLAGS)
LDFLAGS	 = $(FLAGS) -o

OBJECTS	 = main.o NetworkManager.o MessageManager.o mainscreen.o Packet.o Message.o

all: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(EXE)

main.o: $(SRC)/main.cpp NetworkManager.o mainscreen.o Packet.o
	$(CXX) $(CXXFLAGS) $<

NetworkManager.o: $(SRC)/NetworkManager.cpp $(INCLUDE)/NetworkManager.hpp
	$(CXX) $(CXXFLAGS) $<

MessageManager.o: $(SRC)/MessageManager.cpp $(INCLUDE)/MessageManager.hpp
	$(CXX) $(CXXFLAGS) $<

mainscreen.o: $(SRC)/mainscreen.cpp $(INCLUDE)/ui/mainscreen.h
	$(CXX) $(CXXFLAGS) $<

Packet.o: $(SHARED)/Packet.cpp $(SHARED)/Packet.h
	$(CXX) $(CXXFLAGS) $<

Message.o: $(SHARED)/Message.cpp $(SHARED)/Message.h
	$(CXX) $(CXXFLAGS) $<

clean:
	del -q *.exe *.o
