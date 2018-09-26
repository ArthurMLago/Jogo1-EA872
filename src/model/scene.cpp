#include "scene.hpp"

Scene::~Scene(){
	delete player;
	for (int i = enemyList.size() - 1; i >= 0; i--){
		delete enemyList[i];
		enemyList.pop_back();
	}
}
