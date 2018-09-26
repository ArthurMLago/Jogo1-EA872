#include "gameController.hpp"

/*
*Funcao chamada a cada frame do jogo, provavelmente 60 vezes por segundos
*Deve cuidar das funcoes basicas do jogo, como criar mais inimigos bolas, fazer os inimigos se movimentarem,
*verificar colisoes, etc
*/
void GameController::update(){
	if(tempo%120 == 0){
		// cria mais bolas
		currentScene->Enemy* novo_enemy = new Enemy(0.0; float pos_y,get_speed()+(rand())/1.0);;
		currentScene->enemyList.push_back(novo_enemy);

		// move as bolas que ja existem
		for(int i = 0; i<currentScene->enemyList.size();i++){
			currentScene->enemyList[i]->move(1.0, 0.0);
		}
		//Altera a velocidade
		for(int i = 0; i<currentScene->enemyList.size();i++){
			currentScene->enemyList[i]->get_speed();
		}

		int flag = 0;
		// verifica colisoes
		for(int i = 0; i < currentScene->enemyList.size();i++){
			if(currentScene->enemyList[i]->get_pos_x() == currentScene->player->get_pos_x() 
				&&currentScene->enemyList[i]->get_pos_y() == currentScene->player->get_pos_y()){
				flag = 1;
				i = currentScene->enemyList.size();
			}
		}
	}
	tempo++;
}

/*
*Essa funcão atualiza a propriedade viewController para o ponteiro passado.
*/
void GameController::setViewController(ViewController *pointer){
	viewController = pointer;
}

/*
*Essa funcão atualiza a propriedade Setscene para o ponteiro passado.
*/
void GameController::setScene(Scene *pointer){
	currentScene = pointer;
}

/*
*Funcao chamada pelo viewController para avisar o gameController que o usuario apertou uma tecla para
*ir para cima e a posição do jogador deve ser atualizada no model
*/
void GameController::userPressedUp(){
	currentScene->player->move(0.0, 1.0);

}

/* 
*Funcao chamada pelo viewController para avisar o gameController que o usuario apertou uma tecla para
*ir para baixo e a posição do jogador deve ser atualizada no model
*/
void GameController::userPressedDown(){
	currentScene->player->move(0.0, -1.0);
}


