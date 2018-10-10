#pragma once


#include <thread>
#include <ncurses.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "playback.hpp"
#include "../model/scene.hpp"
#include "../controller/gameController.hpp"


class GameController;

class ViewController{
		Audio::Sample *moveSample;
		Audio::Sample *collideSample;
		Audio::Sample *gameOverSample;

		Audio::Player player;

		GameController *gameController;
		Scene *currentScene;

		std::thread *inputThread;

		int screenW, screenH;
		char keyBuffer;
		int control;

	public:
		ViewController();
		~ViewController();
		
		void setGameController(GameController *gameController);
		void setScene(Scene *currentScene);

		void getScreenDimension(int *dst_Width, int *dst_Height);
		
		void playCollisionSound();
		void playMoveSound();
		void showGameOverScreen();
		void drawScene();

		

		void input_thread_routine();



};

