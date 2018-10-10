
#include <thread>
#include "controller/gameController.hpp"


Scene *globalScene;
GameController *gController;

void remoteUpdater();

int main(){

	GameController gameController;
	ViewController viewController;
	Scene currentScene;

	gController = &gameController;
	globalScene = &currentScene;


	viewController.setScene(&currentScene);
	viewController.setGameController(&gameController);

	gameController.setViewController(&viewController);
	gameController.setScene(&currentScene);
	
	
	int larg;
	int alt;
	viewController.getScreenDimension(&larg, &alt);

	currentScene.player = new Player(larg/2,alt/2);

	std::thread first(remoteUpdater);

	while(!gameController.shouldTerminate()){
	//while(1){
		gameController.update();
		viewController.drawScene();
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
	first.join();


}

void remoteUpdater(){
	int socket_fd, connection_fd;
	struct sockaddr_in myself, client;
	socklen_t client_size = (socklen_t)sizeof(client);
	char input_buffer[50];

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	fprintf(stderr, "Socket criado\n");

	myself.sin_family = AF_INET;
	myself.sin_port = htons(18258);
	inet_aton("127.0.0.1", &(myself.sin_addr));
	
	fprintf(stderr, "Tentando abrir porta 18258\n");
	while(bind(socket_fd, (struct sockaddr*)&myself, sizeof(myself)) == 0) {

		fprintf(stderr, "Problemas ao abrir porta 18258\n");
		sleep(1);
	}
	fprintf(stderr, "Abri porta 3001!\n");

	listen(socket_fd, 2);
	fprintf(stderr, "Estou ouvindo na porta 18258!\n");
	
	while(!gController->shouldTerminate()){
		fprintf(stderr, "Vou travar ate receber alguma coisa\n");
		connection_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);

		printf("fez algo\n");
		int closed = 0;
		while(!gController->shouldTerminate() && !closed){
			int expected;
			unsigned char *pointer;
			pointer = globalScene->serialize(&expected);
			int sent = send(connection_fd, pointer, expected, 0);
			if (sent != expected)
				closed = 1;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}


	}
}
