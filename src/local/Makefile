# compiler
CC=g++

# compilation flags
CFLAGS=-g -Wall -std=c++11

# linker flags
LDFLAGS=

INCDIR=../greedy

# .cpp files to compile
SOURCES=local.cpp ../greedy/greedy.cpp main.cpp

# objects that will be generated from .cpp files
OBJECTS=$(SOURCES:.cpp=.o)

# executable name
EXECUTABLE=local

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

.cpp.o:
	$(CC) -I$(INCDIR) $(CFLAGS) $< -c -o $@