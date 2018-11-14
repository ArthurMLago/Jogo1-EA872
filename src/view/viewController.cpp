#include "viewController.hpp"
#include <math.h>

//int ViewController::control = 0;

ViewController::ViewController(){
	// Criar samples de audio:
	moveSample = new Audio::Sample("assets/move.wav");
	collideSample = new Audio::Sample("assets/collision.wav");
	gameOverSample = new Audio::Sample("assets/gameOver.wav");
	moveFailSample = new Audio::Sample("assets/move_fail.wav");
	
	player = new Audio::Player();

	// Inicar ncurses:
	initscr();
	raw();
    noecho();
	curs_set(0);
	getmaxyx(stdscr, this->screenH, this->screenW);
	
	start_color();			/* Start color 			*/
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	
	gameController = NULL;
	currentScene = NULL;
	serverController = NULL;
	// Criar thread que escuta por comandos:
	inputThread = new std::thread(&ViewController::input_thread_routine, this);

	control = 0;

}

ViewController::~ViewController(){
    inputThread->join();
    delete inputThread;

    delete player;	
    delete moveSample;
    delete collideSample;
    delete gameOverSample;

	endwin();
}

void ViewController::setGameController(UserResponder *gameController){
	this->gameController = gameController;
}

void ViewController::setServerController(ServerController *srv){
	this->serverController = srv;
}

void ViewController::setScene(Scene *currentScene){
	this->currentScene = currentScene;
}

void ViewController::getScreenDimension(int *dst_Width, int* dst_Height){
	*dst_Width = screenW;
	*dst_Height = screenH;
}

void ViewController::playCollisionSound(){
	float intensities[2] = {0.8,0.8};
	if (player)
		player->play(collideSample, intensities);
	if (serverController)
		serverController->sendSoundRequestToClients(1,0,0);

}

void ViewController::playMoveSound(){
	float intensities[2] = {0.7,0.7};
	if (player)
		player->play(moveSample, intensities);
	if (serverController)
		serverController->sendSoundRequestToClients(0,0,0);
}

void ViewController::playMoveFailSound(){
	float intensities[2] = {0.7,0.7};
	if (player)
		player->play(moveFailSample, intensities);
	if (serverController)
		serverController->sendSoundRequestToClients(0,0,0);


}

void ViewController::showGameOverScreen(){
	move(screenH/2 - 2, screenW/2 - 20);
	addstr("########################################");
	move(screenH/2 - 1, screenW/2 - 20);
	addstr("############## Voce PERDEU!! ###########");
	move(screenH/2, screenW/2 - 20);
	addstr("###### Pressione Q para sair!! #########");
	move(screenH/2 + 1, screenW/2 - 20);
	addstr("########################################");
	refresh();
	if (player){
		float intensities[2] = {0.7,0.7};
		player->play(gameOverSample, intensities);
	}

	std::this_thread::sleep_for(std::chrono::seconds(5));
}

void ViewController::drawScene(){
	// Limpa tela:
	erase();
	// Desenha inigos:
	for (int i = 0; i < currentScene->enemyList.size(); i++){
		float x_super = currentScene->enemyList[i]->get_pos_x();
		int x = roundf(x_super);
		int y = currentScene->enemyList[i]->get_pos_y();


		move(y,x);
		addch('*');
	}
	for (int i = 0; i < currentScene->playerList.size(); i++){
		move(currentScene->playerList[i]->get_pos_y(), currentScene->playerList[i]->get_pos_x());

		attron(COLOR_PAIR(1));
		addch('P');
		attroff(COLOR_PAIR(1));

	}

	refresh();

}

void ViewController::input_thread_routine(){
	char c;
	timeout(1000);
	while((gameController == NULL) || (!gameController->shouldTerminate())){
		c = getch();
		if (c != ERR && gameController != NULL){
			if (c == 'w'){
				gameController->userPressedUp();
			}else if(c == 's'){
				gameController->userPressedDown();
			}else if(c == 'a'){
				gameController->userPressedLeft();
			}else if(c == 'd'){
				gameController->userPressedRight();
			}else if(c == 'q'){
				gameController->terminate();
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(16));

	}
}
