#pragma once

#include "iuserResponder.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <thread>
#include "../view/viewController.hpp"
#include "../model/scene.hpp"

class ClientController : public UserResponder{
	private:
		std::thread *recebe;
		int socket_fd;
		int server_index;
		int shouldTerminate_Aux;

		ViewController *viewController;
		Scene *currentScene;
	public:
		// COnstrutor: cria socket, cria thread.
		ClientController(const char *Endereco, int porta);
		// Enviam o comando apropriado pelo socket:
		void userPressedUp();
		void userPressedDown();
		void userPressedLeft();
		void userPressedRight();
		void terminate();
		int shouldTerminate();
		void setViewController(ViewController *pointer);
		void setScene(Scene *pointer);
		void receive_thread();

};
