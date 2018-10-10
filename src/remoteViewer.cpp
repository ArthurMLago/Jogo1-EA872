
#include <thread>
#include "view/viewController.hpp"
#include "model/scene.hpp"

void closer();

volatile int finish = 0;
int main(){

	ViewController viewController(0);
	Scene currentScene;
	currentScene.player = new Player(0,0);
	viewController.setScene(&currentScene);
	//std::thread ttthread(closer);
	

	int socket_fd;
	struct sockaddr_in target;
	
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	printf("Socket criado\n");

	target.sin_family = AF_INET;
	target.sin_port = htons(18258);
	inet_aton("127.0.0.1", &(target.sin_addr));
	printf("Tentando conectar\n");
	while(connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
		printf("Problemas na conexao\n");
		sleep(1);
		/*endwin();*/
		/*return 0;*/
	}
	printf("Conectei ao servidor\n");
	while(!finish){
		unsigned char buffer[4096];
		int received = recv(socket_fd, buffer, 4, 0);
		if (received != 4){
			break;
		}
		int n_enemies = ((int*)buffer)[0];
		received = recv(socket_fd, buffer + 4, n_enemies * sizeof(Enemy) + sizeof(Player), 0);
		if (received != n_enemies * sizeof(Enemy) + sizeof(Player)){
			break;
		}
		currentScene.unserialize(buffer);
		//if (getch() == 'q')
			//break;
		viewController.drawScene();
	}
	endwin();

}

void closer(){
	char c;
	while(1){
		   c = getch();
		if (c == 'q'){
			finish = 1;
			break;
		}
	}
}
