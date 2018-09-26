#pragma once

#include <vector>

#include "../model/player.hpp"
#include "../model/enemy.hpp"

class Scene {
	public:
		std::vector<Enemy*> enemyList;
		Player *player;

		~Scene();

		

};
