CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=Lex.cpp
OBJECTS=$(SOURCES:.cpp=.o)
	EXECUTABLE=YAIL2

all: $(SOURCES) $(EXECUTABLE)
		
$(EXECUTABLE): $(OBJECTS) 
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS)
