#pragma once

#include "../model/scene.hpp"
#include "../view/viewController.hpp"

class GameController {
	private:
		Scene *currentScene;
		ViewController *viewController;

	public:
		void setViewController(ViewController *pointer);
		void setScene(Scene *pointer);
		void update();
		void userPressedUp();
		void userPressedDown();
};

