CXX		 = g++

EXE		 = Server

SRC		 = ../src
INCLUDE	 = ../include
UI		 = ./UI
LIB		 = ../lib
SHARED	 = ../../Shared

FLAGS	 = -std=c++20 -I$(INCLUDE) -I$(SHARED) -L$(LIB) -lws2_32
CXXFLAGS = -c -Wall $(FLAGS)
LDFLAGS	 = $(FLAGS) -o

OBJECTS	 = main.o library.o Packet.o ConnectionManager.o Client.o Chatroom.o PromptGenerator.o

all: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(EXE)

main.o: $(SRC)/main.cpp ConnectionManager.o
	$(CXX) $(CXXFLAGS) $<

library.o: $(SRC)/library.cpp $(INCLUDE)/library.h
	$(CXX) $(CXXFLAGS) $<

ConnectionManager.o: $(SRC)/ConnectionManager.cpp $(INCLUDE)/ConnectionManager.h Packet.o Client.o
	$(CXX) $(CXXFLAGS) $<

Packet.o: $(SHARED)/Packet.cpp
	$(CXX) $(CXXFLAGS) $<

Chatroom.o: $(SRC)/Chatroom.cpp $(INCLUDE)/Chatroom.h Client.o
	$(CXX) $(CXXFLAGS) $<

PromptGenerator.o: $(SRC)/PromptGenerator.cpp $(INCLUDE)/PromptGenerator.h
	$(CXX) $(CXXFLAGS) $<

Client.o: $(SRC)/Client.cpp $(INCLUDE)/Client.h
	$(CXX) $(CXXFLAGS) $<

clean:
	del -q *.exe *.o
