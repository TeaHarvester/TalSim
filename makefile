CC = g++
CXXFLAGS = -Wall -g -Iinclude
OBJS = TalSim.o Board.o Position.o Engine.o Piece.o Helpers.o

TalSim: $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) -o $@

TalSim.o: src/Talsim.cpp src/Board.cpp src/Piece.cpp src/Helpers.cpp
	$(CC) $(CXXFLAGS) -c src/TalSim.cpp

Board.o: src/Board.cpp src/Position.cpp src/Engine.cpp src/Piece.cpp src/Helpers.cpp
	$(CC) $(CXXFLAGS) -c src/Board.cpp

Position.o: src/Position.cpp
	$(CC) $(CXXFLAGS) -c src/Position.cpp

Engine.o: src/Engine.cpp src/Position.cpp
	$(CC) $(CXXFLAGS) -c src/Engine.cpp

Piece.o: src/Piece.cpp
	$(CC) $(CXXFLAGS) -c src/Piece.cpp

Helpers.o: src/Helpers.cpp
	$(CC) $(CXXFLAGS) -c src/Helpers.cpp

clean: 
	rm -f *.o *~ 