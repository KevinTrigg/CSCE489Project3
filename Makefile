CC=g++
CXXFLAGS=-g -Wall -Wextra -O0 -pthread
DEPS =   Memory.h
OBJ = main.o Memory.o 
BIN = main 

%.o: %.cpp $(DEPS)
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ 

.PHONY: clean

clean:
	rm -f *.o *~ $(BIN)
