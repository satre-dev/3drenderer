
build:
	gcc -Wall -std=c99 ./src/*.c -o renderer -I /opt/homebrew/cellar/sdl2/2.30.2/include -L /opt/homebrew/cellar/sdl2/2.30.2/lib -l SDL2

run:
	./renderer

clean:
	rm renderer
