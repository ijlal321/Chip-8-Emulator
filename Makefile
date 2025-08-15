INCLUDE = -I ./include
FLAGS = -g

OBJECTS=./build/chip8memory.o ./build/chip8stack.o ./build/chip8keyboard.o ./build/chip8.o ./build/chip8screen.o
all: ${OBJECTS}
	gcc ${FLAGS} ${INCLUDE}  ./src/main.c ${OBJECTS} -L ./lib -lmingw32 -lSDL2main -lSDL2 -o ./bin/main

./build/chip8memory.o:src/chip8memory.c
	gcc ${FLAGS} ${INCLUDE}  ./src/chip8memory.c -c -o ./build/chip8memory.o

./build/chip8stack.o:src/chip8stack.c
	gcc ${FLAGS} ${INCLUDE}  ./src/chip8stack.c -c -o ./build/chip8stack.o

./build/chip8keyboard.o:src/chip8keyboard.c
	gcc ${FLAGS} ${INCLUDE}  ./src/chip8keyboard.c -c -o ./build/chip8keyboard.o

./build/chip8.o:src/chip8.c
	gcc ${FLAGS} ${INCLUDE}  ./src/chip8.c -c -o ./build/chip8.o

./build/chip8screen.o:src/chip8screen.c
	gcc ${FLAGS} ${INCLUDE}  ./src/chip8screen.c -c -o ./build/chip8screen.o

clean:
	rm -f ./build/*

run: all
	./bin/main
