#include "viewController.hpp"
#include <math.h>

//int ViewController::control = 0;

ViewController::ViewController(){
	this->state = 1;
	// Criar samples de audio:
	moveSample = new Audio::Sample("assets/move.wav");
	collideSample = new Audio::Sample("assets/collide.wav");
	gameOverSample = new Audio::Sample("assets/gameOver.wav");
	
	player = new Audio::Player();

	// Inicar ncurses:
	initscr();
	raw();
	curs_set(0);
	getmaxyx(stdscr, this->screenH, this->screenW);
	
	start_color();			/* Start color 			*/
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	
	// Criar thread que escuta por comandos:
	inputThread = new std::thread(&ViewController::input_thread_routine, this);

	control = 0;
}
ViewController::ViewController(int state){
	this->state = state;

	// Inicar ncurses:
	initscr();
	raw();
	curs_set(0);
	getmaxyx(stdscr, this->screenH, this->screenW);

	start_color();			/* Start color 			*/
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);

	
	player = NULL;

	if (state != 0){
		player = new Audio::Player();
		// Criar samples de audio:
		moveSample = new Audio::Sample("assets/move.dat");
		collideSample = new Audio::Sample("assets/collide.dat");
		gameOverSample = new Audio::Sample("assets/gameOver.dat");
		// Criar thread que escuta por comandos:
		inputThread = new std::thread(&ViewController::input_thread_routine, this);
	}

	control = 0;
}

ViewController::~ViewController(){
	if (state){
		inputThread->join();
		delete inputThread;
	
		delete moveSample;
		delete collideSample;
		delete gameOverSample;
	}

	endwin();
}

void ViewController::setGameController(UserResponder *gameController){
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
	//moveSample->position = 0;
	float intensities[2] = {1.0,1.0};
	if (player)
		player->play(moveSample, intensities);

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
		float intensities[2] = {0.0,1.0};
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

	int socket_fd, connection_fd;
	struct sockaddr_in myself, client;
	socklen_t client_size = (socklen_t)sizeof(client);
	char input_buffer[50];

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	fprintf(stderr, "Socket criado\n");

	myself.sin_family = AF_INET;
	myself.sin_port = htons(18259);
	inet_aton("127.0.0.1", &(myself.sin_addr));
	
	fprintf(stderr, "Tentando abrir porta 3001\n");
	while(bind(socket_fd, (struct sockaddr*)&myself, sizeof(myself)) == 0) {

		fprintf(stderr, "Problemas ao abrir porta\n");
		sleep(1);
	}
	fprintf(stderr, "Abri porta 3001!\n");

	listen(socket_fd, 2);
	fprintf(stderr, "Estou ouvindo na porta 3001!\n");


	while ((gameController == NULL) || (!gameController->shouldTerminate())) {
		fprintf(stderr, "Vou travar ate receber alguma coisa\n");
		connection_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);

		int read_bytes = 1;
		while(((gameController == NULL) || (!gameController->shouldTerminate())) && read_bytes == 1){
			char c;
			read_bytes = recv(connection_fd, &c, 1, 0);
			if (c != ERR){
				if (c == 'w'){
					gameController->userPressedUp();
				}else if(c == 's'){
					gameController->userPressedDown();
				}else if(c == 'q'){
					gameController->terminate();
				}
			}
		}
	}
}
