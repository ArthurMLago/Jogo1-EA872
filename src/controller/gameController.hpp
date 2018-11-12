#pragma once

#include "../model/scene.hpp"
#include "../view/viewController.hpp"
#include "iuserResponder.hpp"

class ViewController;

class GameController : public UserResponder{
	private:
		Scene *currentScene;
		ViewController *viewController;
		int tempo = 0; //variável global que conta o tempo começa em 0
		int shouldTerminate_var = 0; // jogo deve acabar

		int screenW;
		int screenH;

	public:
		void setViewController(ViewController *pointer);
		void setScene(Scene *pointer);
		void update();
		void userPressedUp();
		void userPressedDown();
		void userPressedRight();
		void userPressedLeft();
		void userPressedUp(int index);
		void userPressedDown(int index);
		void userPressedRight(int index);
		void userPressedLeft(int index);
		void playerConnected(int index);
		void playerQuit(int index);
		void terminate(); 
		int shouldTerminate();
	
};

