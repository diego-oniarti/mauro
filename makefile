FLTK_INC = C:/libs/fltk/install/include
FLTK_LIB = C:/libs/fltk/install/lib

CXX = g++
CXXFLAGS = -std=c++17 -O2 -I$(FLTK_INC) -I./src
LDLIBS = -L$(FLTK_LIB) -lfltk -lfltk_images -lcomctl32 -lgdi32 -lole32 -luuid -lwinmm -lversion -lshlwapi -lws2_32
LDFLAGS = -mwindows

OUT = bin\main.exe
SRC = $(wildcard src/*.cpp)

$(OUT): main.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

clean:
	del $(OUT)

run: $(OUT)
	$(OUT)
