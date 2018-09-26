#pragma once

#include "../model/scene.hpp"
#include "../view/viewController.hpp"
int tempo = 0; //variável global que conta o tempo começa em 0
int colisao = 0; // variavel global que indica se ouve colisao
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


