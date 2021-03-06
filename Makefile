CF = -Wall -std=c++14
LF = -lallegro -lallegro_primitives -lallegro_audio
CXX = g++
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=obj/%.o)
TRG = asteroids

all: $(TRG)

obj/%.o: src/%.cpp Makefile
	@mkdir -p obj/
	$(CXX) $(CF) $< -c -o $@

$(TRG): $(OBJ) Makefile
	$(CXX) $(OBJ) -o $@ $(LF)

clean:
	rm -rf obj/ $(TRG)
