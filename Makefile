CC = gcc
CXX = g++

CFLAGS   += -g -Wall -std=c99 $(INCFLAGS)
CXXFLAGS += -Wall $(INCFLAGS)
# LDFLAGS  += -shared

INCFLAGS = -I.

SOURCES_C := retroxml.c test.c

OBJECTS := $(SOURCES_C:.c=.o)

EXECUTABLE := retroxml.exe

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(fpic) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(fpic) -c -o $@ $<

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

all: $(EXECUTABLE)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: clean test
