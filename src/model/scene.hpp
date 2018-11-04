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
		unsigned char *serialize(int *n_bytes);
		void unserialize(unsigned char *buffer);

		

};
