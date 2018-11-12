#include "gameController.hpp"



/*
*Funcao chamada a cada frame do jogo, provavelmente 60 vezes por segundos
*Deve cuidar das funcoes basicas do jogo, como criar mais inimigos bolas, fazer os inimigos se movimentarem,
*verificar colisoes, etc
*/

void GameController::update(){
	int larg;
	int alt;
	viewController->getScreenDimension(&larg, &alt);
	float val = alt*((float)(rand())/RAND_MAX);
	// a cada 2 segundos entra no if 
	if(tempo%20 == 0){
		// cria mais bolas
		Enemy* novo_enemy = new Enemy(0.5, val,(0.3 * (float)(rand())/RAND_MAX));
		currentScene->enemyList.push_back(novo_enemy);
	}

	// move as bolas que ja existem
	for(int i = 0; i<currentScene->enemyList.size();i++){
		currentScene->enemyList[i]->move(currentScene->enemyList[i]->get_speed(), 0.0);
	}

	// loop que verificar se ocorreu alguma colisa
	//for(int i = 0; i < currentScene->enemyList.size();i++){
		//// Se ouver colisao entra no if
		//if((currentScene->enemyList[i]->get_pos_x() <= currentScene->player->get_pos_x()+0.5
			//&&currentScene->enemyList[i]->get_pos_x() >= currentScene->player->get_pos_x()-0.5) 
				//&&(currentScene->enemyList[i]->get_pos_y() == currentScene->player->get_pos_y()))
		   //{
				//// Fim do jogo
				//viewController->showGameOverScreen();
				////viewController->playCollisionSound();
				//// condicao para parar o loop
				//i = currentScene->enemyList.size();
		//}
	//}
	for (int i = 0; i < currentScene->playerList.size(); i++){
		for (int j = 0; j < currentScene->enemyList.size(); j++){
			if (currentScene->playerList[i]->get_pos_y() == currentScene->enemyList[j]->get_pos_y()){
				if (currentScene->playerList[i]->get_pos_x() > currentScene->enemyList[j]->get_pos_x() - 0.5){
					if (currentScene->playerList[i]->get_pos_x() < currentScene->enemyList[j]->get_pos_x() + 0.5){
						Player *deleteme = currentScene->playerList[i];
						currentScene->playerList.erase(currentScene->playerList.begin()+i);
						delete deleteme;
						viewController->playCollisionSound();
						
					}
				}
			}
		}
	}
	
	
	// Loop que verifica se os inimigos estao fora da tela
	// Vale uma critica ao delete pois este deveria estar encapsulado em funcoes apropriadas dentro da classe Scene
	for(int i = 0; i < currentScene->enemyList.size();i++){
		if((currentScene->enemyList[i]->get_pos_x() > larg) || (currentScene->enemyList[i]->get_pos_y() > alt)){
			Enemy *deleteme = currentScene->enemyList[i];
			currentScene->enemyList.erase(currentScene->enemyList.begin() + i);
			delete deleteme;
		}
	}
	tempo++;
}

/*
*Essa funcão atualiza a propriedade viewController para o ponteiro passado.
*/
void GameController::setViewController(ViewController *pointer){
	viewController = pointer;

	int larg;
	int alt;
	viewController->getScreenDimension(&larg, &alt);
	screenW = larg;
	screenH = alt;
}

/*
*Essa funcão atualiza a propriedade Setscene para o ponteiro passado.
*/
void GameController::setScene(Scene *pointer){
	currentScene = pointer;
}

void GameController::userPressedUp(){
	fprintf(stderr, "no local player\n");
}
void GameController::userPressedDown(){
	fprintf(stderr, "no local player\n");
}
void GameController::userPressedRight(){
	fprintf(stderr, "no local player\n");
}
void GameController::userPressedLeft(){
	fprintf(stderr, "no local player\n");
}

/*
*Funcao chamada pelo viewController para avisar o gameController que o usuario apertou uma tecla para
*ir para cima e a posição do jogador deve ser atualizada no model
*/
void GameController::userPressedUp(int index){
	int found = 0;
	for (int i = 0; i < currentScene->playerList.size() && !found; i++){
		if (currentScene->playerList[i]->get_socket_index() == index){
			if (currentScene->playerList[i]->get_pos_y() - 1 > 0){
				currentScene->playerList[i]->move(0,-1);
				viewController->playMoveSound();
			}
			found = 1;
		}
	}
}

/* 
*Funcao chamada pelo viewController para avisar o gameController que o usuario apertou uma tecla para
*ir para baixo e a posição do jogador deve ser atualizada no model
*/
void GameController::userPressedDown(int index){
	int found = 0;
	for (int i = 0; i < currentScene->playerList.size() && !found; i++){
		if (currentScene->playerList[i]->get_socket_index() == index){
			currentScene->playerList[i]->move(0,1);
			viewController->playMoveSound();
			found = 1;
		}
	}
}


void GameController::userPressedRight(int index){
	int found = 0;
	for (int i = 0; i < currentScene->playerList.size() && !found; i++){
		if (currentScene->playerList[i]->get_socket_index() == index){
			currentScene->playerList[i]->move(1,0);
			viewController->playMoveSound();
			found = 1;
		}
	}
}
void GameController::userPressedLeft(int index){
	int found = 0;
	for (int i = 0; i < currentScene->playerList.size() && !found; i++){
		if (currentScene->playerList[i]->get_socket_index() == index){
			if (currentScene->playerList[i]->get_pos_x() - 1 > 0.1){
				currentScene->playerList[i]->move(-1,0);
				viewController->playMoveSound();
			}
			found = 1;
		}
	}
}

void GameController::playerQuit(int index){
	fprintf(stderr, "player quit %d\n", index);
	int found = 0;
	for (int i = 0; i < currentScene->playerList.size() && !found; i++){
		if (currentScene->playerList[i]->get_socket_index() == index){
			delete currentScene->playerList[i];
			currentScene->playerList.erase(currentScene->playerList.begin()+i);
			found = 1;
		}
	}
}

/* 
*Funcao que troca o valor da variavel shouldterminate
*/
void GameController::terminate(){
	shouldTerminate_var = 1;
}
/* 
*Funcao que da o retorno do valor de shouldTerminate
*/
int GameController::shouldTerminate(){
	return shouldTerminate_var;
}

void GameController::playerConnected(int index){
	int larg;
	int alt;
	viewController->getScreenDimension(&larg, &alt);
	Player *newPlayer = new Player(rand() % (larg + 1), rand() % (alt + 1), index);
	currentScene->playerList.push_back(newPlayer);
}
