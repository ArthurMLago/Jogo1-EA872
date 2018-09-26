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
	if(tempo%120 == 0){
		// cria mais bolas
		Enemy* novo_enemy = new Enemy(0.0, val,(rand())/1.0);
		currentScene->enemyList.push_back(novo_enemy);
	}

	// A cada 1 segundo entra no if
	if(tempo%60 == 0){	
		// move as bolas que ja existem
		for(int i = 0; i<currentScene->enemyList.size();i++){
			currentScene->enemyList[i]->move(1.0, 0.0);
		}
	}
	// loop que verificar se ocorreu alguma colisa
	for(int i = 0; i < currentScene->enemyList.size();i++){
		// Se ouver colisao entra no if
		if((currentScene->enemyList[i]->get_pos_x() <= currentScene->player->get_pos_x()+0.5
			&&currentScene->enemyList[i]->get_pos_x() >= currentScene->player->get_pos_x()-0.5) 
				&&(currentScene->enemyList[i]->get_pos_y() <= currentScene->player->get_pos_y()+0.5
					&&currentScene->enemyList[i]->get_pos_y() >= currentScene->player->get_pos_y()-0.5)){
				// Fim do jogo
				viewController->showGameOverScreen();
				// condicao para parar o loop
				i = currentScene->enemyList.size();
		}
	}
	tempo++;
}

/*
*Essa funcão atualiza a propriedade viewController para o ponteiro passado.
*/
void GameController::setViewController(ViewController *pointer){
	int larg;
	int alt;
	viewController->getScreenDimension(&larg, &alt);
	viewController = pointer;
}

/*
*Essa funcão atualiza a propriedade Setscene para o ponteiro passado.
*/
void GameController::setScene(Scene *pointer){
	int larg;
	int alt;
	viewController->getScreenDimension(&larg, &alt);
	currentScene = pointer;
}

/*
*Funcao chamada pelo viewController para avisar o gameController que o usuario apertou uma tecla para
*ir para cima e a posição do jogador deve ser atualizada no model
*/
void GameController::userPressedUp(){
	int larg;
	int alt;
	viewController->getScreenDimension(&larg, &alt);
	currentScene->player->move(0.0, 1.0);
	viewController->playMoveSound();

}

/* 
*Funcao chamada pelo viewController para avisar o gameController que o usuario apertou uma tecla para
*ir para baixo e a posição do jogador deve ser atualizada no model
*/
void GameController::userPressedDown(){
	int larg;
	int alt;
	viewController->getScreenDimension(&larg, &alt);
	currentScene->player->move(0.0, -1.0);
	viewController->playMoveSound();
}

