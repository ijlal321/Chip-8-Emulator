INCLUDE = -I ./include
FLAGS = -g

OBJECTS=./build/chip8memory.o
all: ${OBJECTS}
	gcc ${FLAGS} ${INCLUDE}  ./src/main.c ${OBJECTS} -L ./lib -lmingw32 -lSDL2main -lSDL2 -o ./bin/main

./build/chip8memory.o:src/chip8memory.c
	gcc ${FLAGS} ${INCLUDE}  ./src/chip8memory.c -c -o ./build/chip8memory.o

clean:
	rm -f ./build/*

run: all
	./bin/main.exe