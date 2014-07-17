CC=g++
CFLAGS=-c -std=c++0x -Wall
LDFLAGS=
SOURCES=Lex.cpp Parser.cpp ILC.cpp
OBJECTS=$(SOURCES:.cpp=.o)
	EXECUTABLE=YAIL2

all: $(SOURCES) $(EXECUTABLE)
		
$(EXECUTABLE): $(OBJECTS) 
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS)
