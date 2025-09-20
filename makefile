Z3_INC = C:/libs/z3/include
Z3_LIB = C:/libs/z3/bin
SRC = main.cpp
TARGET = bin/main

CXX = g++
CXXFLAGS = -std=c++17 -O2 -I$(Z3_INC)
LDFLAGS = -L$(Z3_LIB) -lz3
OUT = $(TARGET).exe

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

clean:
	del bin\main.exe

run: $(OUT)
	bin\main.exe
