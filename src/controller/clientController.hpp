#pragma once

#include "iuserResponder.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

class ClientController : public UserResponder{
	private:
		std::thread *recebe;
		int socket;
		int server_index;
		int shouldTerminate_Aux;
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

};
