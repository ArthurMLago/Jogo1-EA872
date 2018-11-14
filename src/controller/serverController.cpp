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

	action_bytes_pending.resize(socket_list.size());
	action_buffer.resize(socket_list.size());
	for (int i = 0; i < socket_list.size(); i++){
		action_bytes_pending[i] = 0;
		action_buffer[i] = (unsigned char*)malloc(4096);
	}

	recvThread = new std::thread(&ServerController::recvThreadRoutine, this);
	sendThread = new std::thread(&ServerController::sendThreadRoutine, this);

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
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
	}
}

void ServerController::sendThreadRoutine(){
	std::vector<unsigned char*> buffer_vet(socket_list.size());
	std::vector<int> len_vet(socket_list.size(), 0);
	std::vector<int> sent_vet(socket_list.size(), 0);

	//buffer_vet = (unsigned char **)malloc(socket_list.size() * sizeof(unsigned char *));
	for (int i = 0; i < socket_list.size(); i++){
		buffer_vet[i] = (unsigned char *)malloc(65536);
		memcpy(buffer_vet[i], &(action_bytes_pending[i]), 4);
		memcpy(buffer_vet[i] + 4, action_buffer[i], action_bytes_pending[i]);
		len_vet[i] = 4 + action_bytes_pending[i];
		action_bytes_pending[i] = 0;
		len_vet[i] += currentScene->serialize(buffer_vet[i] + len_vet[i]);
		sent_vet[i] = 0;

	}
	//len_vet = (int*)malloc(socket_list.size() * sizeof(int));
	//sent_vet = (int*)malloc(socket_list.size() * sizeof(int));
	while (!gController->shouldTerminate()){
		for (int i = 0; i < socket_list.size(); i++){
			if (socket_list[i] != -1){
				int n_sent = send(socket_list[i], buffer_vet[i] + sent_vet[i], len_vet[i] - sent_vet[i], 0);
				if ((n_sent == -1) || (n_sent == 0)){
					if (n_sent == -1){
						fprintf(stderr, "send for client %d returned -1, error: %d : %s\n", i, errno, strerror(errno));
					}else{
						fprintf(stderr, "send for client %d returned less bytes than expected %d\n", i, n_sent);
						close(socket_list[i]);
						socket_list[i] = -1;
						gController->playerQuit(i);
						// free it's buffer:
						//free(buffer_vet[i]);
						// delete it from all vectors:
						//buffer_vet.erase(buffer_vet.begin() + i);
						//len_vet.erase(len_vet.begin() + i);
						//sent_vet.erase(sent_vet.begin() + i);
					}
				}else{
					sent_vet[i] += n_sent;
					if (sent_vet[i] >= len_vet[i]){
						memcpy(buffer_vet[i], &(action_bytes_pending[i]), 4);
						memcpy(buffer_vet[i] + 4, action_buffer[i], action_bytes_pending[i]);
						len_vet[i] = 4 + action_bytes_pending[i];
						action_bytes_pending[i] = 0;
						len_vet[i] += currentScene->serialize(buffer_vet[i] + len_vet[i]);
						sent_vet[i] = 0;
					}
				}
			}

		}
		std::this_thread::sleep_for(std::chrono::milliseconds(30));

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


void ServerController::sendSoundRequestToClients(unsigned char audio_id, float pos_x, float pos_y){
	for (int i = 0; i < socket_list.size(); i++){
		char id = 0x28;
		memcpy(action_buffer[i] + action_bytes_pending[i]    , &id, 1);
		memcpy(action_buffer[i] + action_bytes_pending[i] + 1, &audio_id, 1);
		memcpy(action_buffer[i] + action_bytes_pending[i] + 2, &pos_x, 4);
		memcpy(action_buffer[i] + action_bytes_pending[i] + 6, &pos_y, 4);
		action_bytes_pending[i] += 10;
	}
}
