#pragma once

#include <stdlib.h>
#include <string.h>
#include <vector>

#include "../model/player.hpp"
#include "../model/enemy.hpp"

class Scene {
	public:
		std::vector<Enemy*> enemyList;
		std::vector<Player*> playerList;

		~Scene();
		int serialize(unsigned char* result);
		void unserialize(unsigned char *buffer);

		

};
