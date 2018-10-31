#pragma once


#include <thread>
#include <ncurses.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Playback.hpp"
#include "../model/scene.hpp"
#include "../controller/iuserResponder.hpp"


class GameController;

class ViewController{
		Audio::Sample *moveSample;
		Audio::Sample *collideSample;
		Audio::Sample *gameOverSample;

		Audio::Player *player;

		UserResponder *gameController;
		Scene *currentScene;

		std::thread *inputThread;

		int screenW, screenH;
		char keyBuffer;
		int control;
		int state;

	public:
		ViewController();
		ViewController(int state);
		~ViewController();
		
		void setGameController(UserResponder *gameController);
		void setScene(Scene *currentScene);

		void getScreenDimension(int *dst_Width, int *dst_Height);
		
		void playCollisionSound();
		void playMoveSound();
		void showGameOverScreen();
		void drawScene();

		

		void input_thread_routine();



};

