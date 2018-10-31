#include "serverController.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

int ServerController::waitForConnections(){
	int server_fd;
	struct sockaddr_in myself, client;

	// Create socket:	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	// Define which interface and port to listen for connections:
	myself.sin_family = AF_INET;
	myself.sin_port = htons(listen_port);
	inet_aton(listen_address, &(myself.sin_addr));

	// Bind our socket to the interface address and port:
	while(bind(server_fd, (struct sockaddr*)&myself, sizeof(myself)) == 0){
		fprintf(stderr, "Problemas ao abrir porta %d na interface %s\n", listen_port, listen_address);
		fprintf(stderr, "\tError %d : %s\n", errno, strerror(errno));
		sleep(1);
	}
	listen(server_fd, 5);
}
