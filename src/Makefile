# Makefile for main.cpp

# Compiler
CC = g++

# Compiler flags
CFLAGS = -std=c++11 -Wall

# Source file
SRC = ims.cpp

# Output executable
OUT = ims

# Libraries
LIBS = -lsimlib -lm
# Add any other libraries as needed, using the -l flag

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)

run:
	./ims