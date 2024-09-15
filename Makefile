build:
	gcc -Wall -std=c99 ./src/*.c  -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lm -o renderer

build_linux:
	gcc -Wall -std=c99 ./src/*.c -I/libs/SDL/build -L/usr/local/lib/cmake/ -lSDL2 -lm -o renderer

run:
	./renderer

clean:
	rm renderer

build_and_run:
	rm renderer
	gcc -Wall -std=c99 ./src/*.c  -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lm -o renderer
	./renderer

build_and_run_linux:
	rm renderer
	gcc -Wall -std=c99 ./src/*.c -I/libs/SDL/build -L/usr/local/lib/cmake/ -lSDL2 -lm -o renderer
	./renderer