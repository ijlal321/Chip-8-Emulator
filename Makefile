INCLUDE = -I ./include
FLAGS = -g

all: 
	gcc ${FLAGS} ${INCLUDE}  ./src/main.c -L ./lib -lmingw32 -lSDL2main -lSDL2 -o ./bin/main

run: all
	./bin/main.exe