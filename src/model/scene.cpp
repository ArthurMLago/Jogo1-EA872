#include "scene.hpp"

Scene::~Scene(){
	for (int i = enemyList.size() - 1; i >= 0; i--){
		delete enemyList[i];
		enemyList.pop_back();
	}
	for (int i = playerList.size() - 1; i >= 0; i--){
		delete playerList[i];
		playerList.pop_back();
	}
}

int Scene::serialize(unsigned char* result){
	int offset = 0;
	int n_players = playerList.size();
	int n_enemies = enemyList.size();

	// Copy the number of players to the buffer:
	memcpy(result + offset, &n_players, sizeof(int));
	offset += sizeof(int);
	// Copy all players to the buffer:
	for (int i = 0; i < n_players; i++){
		memcpy(result + offset, playerList[i], sizeof(Player));
		offset += sizeof(Player);
	}


	// Copy the number of enemies to the buffer:
	memcpy(result + offset, &n_enemies, sizeof(int));
	offset += sizeof(int);
	// Copy all enemies to the buffer:
	for (int i = 0; i < n_enemies; i++){
		memcpy(result + offset, enemyList[i], sizeof(Enemy));
		offset += sizeof(Enemy);
	}
	//memcpy(result + offset, player, sizeof(Player));
	//offset += sizeof(Player);
	return offset;
}

void Scene::unserialize(unsigned char *buffer){
	int n_players;
	int n_enemies;
	int offset = 0;

	// Get number of players:
	memcpy(&n_players, buffer + offset, sizeof(int));
	offset += sizeof(int);
	// Free old players:
	for (int i = 0; i < playerList.size(); i++){
		free(playerList[i]);
	}
	// Fill vector with new players:
	playerList.resize(n_players);
	for (int i = 0; i < n_players; i++){
		playerList[i] = (Player *)malloc(sizeof(Player));
		memcpy(playerList[i], buffer + offset, sizeof(Player));
		offset += sizeof(Player);
	}
	/////
	// Get number of enemies:
	memcpy(&n_enemies, buffer + offset, sizeof(int));
	offset += sizeof(int);
	// Free old enemies:
	for (int i = 0; i < enemyList.size(); i++){
		free(enemyList[i]);
	}
	// Fill vector with new enemies:
	enemyList.resize(n_enemies);
	for (int i = 0; i < n_enemies; i++){
		enemyList[i] = (Enemy *)malloc(sizeof(Enemy));
		memcpy(enemyList[i], buffer + offset, sizeof(Enemy));
		offset += sizeof(Enemy);
	}
}
