#include "serverController.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

ServerController::ServerController(const char *listen_address, int listen_port){
	this->listen_address = listen_address;
	this->listen_port = listen_port;
}

ServerController::~ServerController(){
	for (int i = 0; i < socket_list.size(); i++){
		if (socket_list[i] != -1){
			int n_read = 128;
			char buff[128];
			while (n_read > 0){
				n_read = recv(socket_list[i], buff, 128, 0);
				if (n_read == -1){
					fprintf(stderr, "error : %s\n", strerror(errno));
				}
				fprintf(stderr, "%d n_read = %d\n", socket_list[i], n_read);
			}
			close(socket_list[i]);
		}
	}
	recvThread->join();
	delete recvThread;
}

int ServerController::waitForConnections(){
	int server_fd;
	struct sockaddr_in myself;

	// Create socket:	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	// Make it ignore TIME-WAIT:
	int enable = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		fprintf(stderr, "setsockopt(SO_REUSEADDR) failed\n");

	// Define which interface and port to listen for connections:
	myself.sin_family = AF_INET;
	myself.sin_port = htons(listen_port);
	//inet_aton(listen_address, &(myself.sin_addr));
	myself.sin_addr.s_addr = htonl (INADDR_ANY);

	// Bind our socket to the interface address and port:
	while(!gController->shouldTerminate() && bind(server_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0){
		fprintf(stderr, "Problemas ao abrir porta %d na interface %s\n", listen_port, listen_address);
		fprintf(stderr, "\tError %d : %s\n", errno, strerror(errno));
		sleep(1);
	}
	// Make this socket non-blocking:
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	// Start listening for incoming connections:
	listen(server_fd, 5);

	int n_sleeps = 0;
	while(!gController->shouldTerminate() && n_sleeps < CONNECT_SECONDS){
		int client_fd;
		struct sockaddr_in client;
		socklen_t client_size = (socklen_t)sizeof(client);
		// Accept new client connection:
		client_fd = accept4(server_fd, (struct sockaddr *)&client, &client_size, O_NONBLOCK); //not POSIX, should fcntl after instead
		if (client_fd != -1){
			int index = socket_list.size();
			int n_written = 0;
			n_written = write(client_fd, &index, sizeof(int));
			if (n_written == sizeof(int)){
				socket_list.push_back(client_fd);
				fprintf(stderr, "Client succesfully connected\n");
				gController->playerConnected(index);
				vController->drawScene();
			}else{
				fprintf(stderr, "Could not send index to connected sokcet\n");
			}
		}else{
			sleep(1);
			n_sleeps++;
		}
	}

	close(server_fd);

	//while(!gController->shouldTerminate()){
		//for (int i = 0; i < socket_list.size(); i++){
			//int n_written;
			//const char *msg = "ping\n";
			//n_written = send(socket_list[i], msg, 5, 0);
		//}
		//sleep(1);
	//}
	fprintf(stderr, "Finishing connecting stage, %lu clients connected\n", socket_list.size());

	recvThread = new std::thread(&ServerController::recvThreadRoutine, this);

	return socket_list.size();
}

void ServerController::recvThreadRoutine(){
	fprintf(stderr, "receive thread started\n");
	while(!gController->shouldTerminate()){
		for (int i = 0; i < socket_list.size(); i++){
			if (socket_list[i] != -1){
				unsigned char received;
				int n_recv = recv(socket_list[i], &received, 1, 0);
				if (n_recv == 1){
					if (received == 'w'){
						gController->userPressedUp(i);
					}else if (received == 'd'){
						gController->userPressedRight(i);
					}else if (received == 'a'){
						gController->userPressedLeft(i);
					}else if(received == 's'){
						gController->userPressedDown(i);
					}else if(received == 'q'){
						gController->playerQuit(i);
					}
				}else if (n_recv == 0){
					close(socket_list[i]);
					socket_list[i] = -1;
					gController->playerQuit(i);
				}
			}
		}
	}
}

void ServerController::sendThreadRoutine(){
	unsigned char **buffer_vet;
	int *len_vet;
	int *sent_vet;
	while (!gController->shouldTerminate()){
		for (int i = 0; i < socket_list.size(); i++){
			int n_sent = send(socket_list[i], buffer_vet[i] + sent_vet[i], len_vet[i] - sent_vet[i], 0);
			if (n_sent != len-vet[i] - sent_vet[i]){

			}

		}
	}
	
}

void ServerController::setGameController(GameController *controller){
	gController = controller;
}

void ServerController::setCurrentScene(Scene *scene){
	currentScene = scene;
}

void ServerController::setViewController(ViewController *controller){
	vController = controller;
}
