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
#include "../controller/iuserResponder.hpp"
#include "../controller/serverController.hpp"


class GameController;
class ServerController;

class ViewController{
		Audio::Sample *moveSample;
		Audio::Sample *collideSample;
		Audio::Sample *gameOverSample;
		Audio::Sample *moveFailSample;

		Audio::Player *player;

		UserResponder *gameController;
		Scene *currentScene;
		ServerController *serverController;

		std::thread *inputThread;

		int screenW, screenH;
		char keyBuffer;
		int control;

	public:
		ViewController();
		~ViewController();
		
		void setGameController(UserResponder *gameController);
		void setScene(Scene *currentScene);
		void setServerController(ServerController *srv);

		void getScreenDimension(int *dst_Width, int *dst_Height);
		
		void playCollisionSound();
		void playMoveSound();
        void playMoveFailSound();
		void showGameOverScreen();
		void drawScene();

		void input_thread_routine();



};

