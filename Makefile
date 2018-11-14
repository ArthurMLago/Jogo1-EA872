CCFLAGS= -O3 -std=c++11 -I.

all: build build/game
	#./build/game

build/game: build/enemy.o build/player.o build/scene.o build/gameController.o build/serverController.o build/clientController.o build/viewController.o build/playback.o src/main.cpp
	g++ $(CCFLAGS) build/enemy.o build/player.o build/scene.o build/gameController.o build/serverController.o build/clientController.o build/viewController.o build/playback.o src/main.cpp -o build/game libsndfile.a -lportaudio -lncurses -lpthread -lasound
	#g++ $(CCFLAGS) build/enemy.o build/player.o build/scene.o build/gameController.o build/serverController.o build/clientController.o build/viewController.o build/playback.o src/main.cpp -o build/game libsndfile.a libportaudio.a -lncurses -lpthread -lasound

build/enemy.o: src/model/enemy.cpp src/model/enemy.hpp
	g++ $(CCFLAGS) src/model/enemy.cpp -c -o build/enemy.o

build/player.o: src/model/player.cpp src/model/player.hpp
	g++ $(CCFLAGS) src/model/player.cpp -c -o build/player.o

build/scene.o: src/model/scene.cpp src/model/scene.hpp
	g++ $(CCFLAGS) src/model/scene.cpp -c -o build/scene.o

build/gameController.o: src/controller/gameController.cpp src/controller/gameController.hpp src/controller/iuserResponder.hpp
	g++ $(CCFLAGS) src/controller/gameController.cpp -c -o build/gameController.o

build/serverController.o: src/controller/serverController.cpp src/controller/serverController.hpp
	g++ $(CCFLAGS) src/controller/serverController.cpp -c -o build/serverController.o

build/clientController.o: src/controller/iuserResponder.hpp src/controller/clientController.cpp src/controller/clientController.hpp
	g++ $(CCFLAGS) src/controller/clientController.cpp -c -o build/clientController.o

build/playback.o: src/view/playback.cpp src/view/playback.hpp
	g++ $(CCFLAGS) src/view/playback.cpp -c -o build/playback.o

build/viewController.o: src/view/viewController.cpp src/view/viewController.hpp
	g++ $(CCFLAGS) src/view/viewController.cpp -c -o build/viewController.o

build:
	mkdir build

clean:
	rm build/*

