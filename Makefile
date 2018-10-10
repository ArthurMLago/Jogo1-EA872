all: build build/game build/remoteViewer build/remoteKeyboard
	#./build/game

build/remoteKeyboard: src/remoteKeyboard.c
	gcc src/remoteKeyboard.c -o build/remoteKeyboard -lncurses

build/remoteViewer: build/enemy.o build/player.o build/scene.o build/gameController.o build/viewController.o build/playback.o src/remoteViewer.cpp
	g++ -g3 -O0 src/remoteViewer.cpp build/scene.o build/viewController.o -o build/remoteViewer build/gameController.o build/playback.o build/player.o build/enemy.o libsndfile.a -lncurses -lpthread -lportaudio

build/game: build/enemy.o build/player.o build/scene.o build/gameController.o build/viewController.o build/playback.o src/main.cpp
	g++ -g3 -O0 build/enemy.o build/player.o build/scene.o build/gameController.o build/viewController.o build/playback.o src/main.cpp -o build/game libsndfile.a -lportaudio -lncurses -lpthread

build/enemy.o: src/model/enemy.cpp src/model/enemy.hpp
	g++ -g3 -O0 src/model/enemy.cpp -c -o build/enemy.o

build/player.o: src/model/player.cpp src/model/player.hpp
	g++ -g3 -O0 src/model/player.cpp -c -o build/player.o

build/scene.o: src/model/scene.cpp src/model/scene.hpp
	g++ -g3 -O0 src/model/scene.cpp -c -o build/scene.o

build/gameController.o: src/controller/gameController.cpp src/controller/gameController.hpp
	g++ -g3 -O0 src/controller/gameController.cpp -c -o build/gameController.o

build/playback.o: src/view/Playback.cpp src/view/Playback.hpp
	g++ -g3 -O0 src/view/Playback.cpp -c -o build/playback.o

build/viewController.o: src/view/viewController.cpp src/view/viewController.hpp
	g++ -g3 -O0 src/view/viewController.cpp -c -o build/viewController.o

build:
	mkdir build

clean:
	rm build/*

