#pragma once

#include "../model/scene.hpp"
#include "gameController.hpp"
#include "../view/viewController.hpp"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>


#define CONNECT_SECONDS 10


class ServerController{
	private:
		const char *listen_address;
		int listen_port;

		std::vector<int> socket_list;

		GameController *gController;
		ViewController *vController;
		Scene *currentScene;

		std::vector<int> action_bytes_pending;
		std::vector<unsigned char*> action_buffer;

		std::thread *sendThread;
		std::thread *recvThread;
	public:
		ServerController(const char *listen_address, int listen_port);
		~ServerController();
		// Waits CONNECT_SECONDS for players to connect, will call GameController::playerConnected for each player :
		// Each connected player will receive 4 bytes indicating it`s ID in the game
		int waitForConnections();
		
		// Thread that constantly sends the current screen for peers and receives their input:
		void recvThreadRoutine();
		void sendThreadRoutine();
		
		// Updates current scene to send it to peers:
		void setCurrentScene(Scene *scene);

		// Updates current Game Controller:
		void setGameController(GameController *controller);

		// Updates current view Controller:
		// We need this to be able to call draw screen from inside waitForConnections
		// This is kinda messy, but fucking works
		void setViewController(ViewController *controller);
};

