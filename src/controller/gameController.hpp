#pragma once

#include "../model/scene.hpp"
#include "../view/viewController.hpp"

class GameController {
	private:
		Scene *currentScene;
		ViewController *viewController;
		int tempo = 0; //variável global que conta o tempo começa em 0

	public:
		void setViewController(ViewController *pointer);
		void setScene(Scene *pointer);
		void update();
		void userPressedUp();
		void userPressedDown();
};


