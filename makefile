Z3_INC = C:/libs/z3/include
Z3_LIB = C:/libs/z3/bin

FLTK_INC = C:/libs/fltk/install/include
FLTK_LIB = C:/libs/fltk/install/lib

CXX = g++
CXXFLAGS = -std=c++17 -O2 -I$(Z3_INC) -I$(FLTK_INC) -I./src
LDLIBS = -L$(Z3_LIB) -lz3 -L$(FLTK_LIB) -lfltk -lfltk_images -lcomctl32 -lgdi32 -lole32 -luuid -lwinmm -lversion -lshlwapi -lws2_32
LDFLAGS = -mwindows

OUT = bin\main.exe
SRC = $(wildcard src/*.cpp)

$(OUT): main.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

clean:
	del $(OUT)

run: $(OUT)
	$(OUT)
