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

unsigned char *Scene::serialize(int *n_bytes){
	static unsigned char result[4096];
	int offset = 0;
	int n_enemies = enemyList.size();
	//result[offset++] = enemyList.size();
	memcpy(result, &n_enemies, sizeof(int));
	offset += sizeof(int);
	for (int i = 0; i < n_enemies; i++){
		memcpy(result + offset, enemyList[i], sizeof(Enemy));
		offset += sizeof(Enemy);
	}
	//memcpy(result + offset, player, sizeof(Player));
	//offset += sizeof(Player);
	*n_bytes = offset;
	return result;
}

void Scene::unserialize(unsigned char *buffer){
	int n_enemies;
	int offset = 0;;
	memcpy(&n_enemies, buffer, sizeof(int));
	offset += sizeof(int);

	for (int i = 0; i < enemyList.size(); i++){
		free(enemyList[i]);
	}
	enemyList.resize(n_enemies);
	for (int i = 0; i < n_enemies; i++){
		enemyList[i] = (Enemy *)malloc(sizeof(Enemy));
		memcpy(enemyList[i], buffer + offset, sizeof(Enemy));
		offset += sizeof(Enemy);
	}
	//memcpy(player, buffer + offset, sizeof(Player));
}
