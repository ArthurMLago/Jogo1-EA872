#include "viewController.hpp"

//int ViewController::control = 0;

ViewController::ViewController(){
	moveSample = new Audio::Sample("assets/move.dat");
	collideSample = new Audio::Sample("assets/collide.dat");
	gameOverSample = new Audio::Sample("assets/gameOver.dat");

	initscr();
	raw();
	curs_set(0);
	getmaxyx(stdscr, this->screenH, this->screenW);

	inputThread = new std::thread(&ViewController::input_thread_routine, this);

	control = 0;
}

ViewController::~ViewController(){
	inputThread->join();
	delete inputThread;

	delete moveSample;
	delete collideSample;
	delete gameOverSample;
}

void ViewController::setGameController(GameController *gameController){
	this->gameController = gameController;
}

void ViewController::setScene(Scene *currentScene){
	this->currentScene = currentScene;
}

void ViewController::getScreenDimension(int *dst_Width, int* dst_Height){
	*dst_Width = screenW;
	*dst_Height = screenH;
}

void ViewController::playCollisionSound(){

}

void ViewController::playMoveSound(){
	moveSample->set_position(0);
	player.play(moveSample);

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

	std::this_thread::sleep_for(std::chrono::seconds(5));
}

void ViewController::drawScene(){
	// Limpa tela:
	clear();
	// Desenha inigos:
	for (int i = 0; i < currentScene->enemyList.size(); i++){
		int x = currentScene->enemyList[i]->get_pos_x();
		int y = currentScene->enemyList[i]->get_pos_y();

		move(y,x);
		echochar('*');
	}
	move(currentScene->player->get_pos_x(), currentScene->player->get_pos_y());
	echochar('P');

	refresh();

}

void ViewController::input_thread_routine(){
	char c;
	while(control == 0){
		c = getch();
		if (c != ERR){
			if (c == 'w'){
				//gameController->userPressedUp();
				printf("bobobobob\n");
			}else if(c == 's'){
				//gameController->userPressedDown();
			}else if(c == 'q'){
				//showGameOverScreen();
				//gameController->terminateGame();
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(16));

	}
}
