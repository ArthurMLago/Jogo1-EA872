#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include "controller/gameController.hpp"
#include "controller/serverController.hpp"
#include <signal.h>




int main(int argc, char **argv){
	signal(SIGPIPE, SIG_IGN);
	unsigned long int seed = time(NULL);
	fprintf(stderr,"used seed: %ld\n", seed);
	srand(seed);

	fprintf(stderr, "argc = %d\n", argc);
	for (int i = 0; i < argc; i++){
		fprintf(stderr, "%s\n",argv[i]);
	}

	fprintf(stderr, "comparison: %d\n", strcmp(argv[1], "client"));
	int server = 1;
	if (argc >= 2 && !strcmp(argv[1], "client")){
		fprintf(stderr, "gooing to client mode\n");
		server = 0;
	}

	if (server){
		GameController gameController;
		ViewController viewController;
		Scene currentScene;
		ServerController serverController("127.0.0.1", 7823);

		viewController.setScene(&currentScene);
		viewController.setGameController(&gameController);

		gameController.setViewController(&viewController);
		gameController.setScene(&currentScene);
		
		
		int larg;
		int alt;
		viewController.getScreenDimension(&larg, &alt);

		//currentScene.player = new Player(larg/2,alt/2);


		serverController.setGameController(&gameController);
		serverController.setViewController(&viewController);
		serverController.setCurrentScene(&currentScene);

		serverController.waitForConnections();

		while(!gameController.shouldTerminate()){
		//while(1){
			gameController.update();
			viewController.drawScene();
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
	}else{
		//ClientController 

	}

}
