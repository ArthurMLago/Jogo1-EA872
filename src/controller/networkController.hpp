#pragma once

#define CONNECT_SECONDS 20


class ServerController{
	private:
		std::vector<int> socket_list;
		Scene *currentScene;
	public:
		// Waits CONNECT_SECONDS for players to connect, will call GameController::playerConnected for each player :
		// Each connected player will receive 4 bytes indicating it`s ID in the game
		int waitForConnections();
		
		// Thread that constantly sends the current screen for peers and receives their input:
		void socketMonitorThread();
		
		// Updates current scene to send it to peers:
		void setCurrentScene(Scene *scene);
}

