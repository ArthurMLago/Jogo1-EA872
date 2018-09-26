all: build build/game
	#./build/game

build/game: build/enemy.o build/player.o build/scene.o build/gameController.o build/viewController.o build/playback.o src/main.cpp
	g++ build/enemy.o build/player.o build/scene.o build/gameController.o build/viewController.o build/playback.o src/main.cpp -o build/game -lportaudio -lncurses -lpthread

build/enemy.o: src/model/enemy.cpp src/model/enemy.hpp
	g++ src/model/enemy.cpp -c -o build/enemy.o

build/player.o: src/model/player.cpp src/model/player.hpp
	g++ src/model/player.cpp -c -o build/player.o

build/scene.o: src/model/scene.cpp src/model/scene.hpp
	g++ src/model/scene.cpp -c -o build/scene.o

build/gameController.o: src/controller/gameController.cpp src/controller/gameController.hpp
	g++ src/controller/gameController.cpp -c -o build/gameController.o

build/playback.o: src/view/playback.cpp src/view/playback.hpp
	g++ src/view/playback.cpp -c -o build/playback.o

build/viewController.o: src/view/viewController.cpp src/view/viewController.hpp
	g++ src/view/viewController.cpp -c -o build/viewController.o

build:
	mkdir build

clean:
	rm build/*

