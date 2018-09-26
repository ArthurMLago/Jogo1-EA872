#pragma once

#include "../model/scene.hpp"
#include "../view/viewController.hpp"

// Resolver depenencia circular:
class Scene;
class ViewController;

class GameController {
	private:
		Scene *currentScene;
		ViewController *viewController;


	public:

		/* Como o gameController tem que se comunicar com o viewController para fazer coisas como exibir uma
		 * animação de Game Over, tocar algum som, etc, etc, precisamos de um ponteiro para o viewController do jogo,
		 * assim poderemos chamar seus métodos.
		 * Essa funcão deve simplesmente atualizar a propriedade viewController para o ponteiro passado.
		 */
		void setViewController(ViewController *pointer);

		/* Mesmo raciocinio da função setViewController, mas agora para a cena do jogo, que contém informação de todos
		 * os inimigos e do jogador.
		 * Deve atualizar a propriedade currentScene para o ponteiro passado.
		 */
		void setScene(Scene *pointer);

		/* Funcao chamada a cada frame do jogo, provavelmente 60 vezes por segundos
		 * Deve cuidar das funcoes basicas do jogo, como criar mais inimigos, fazer os inimigos se movimentarem,
		 * verificar colisoes, etc
		 */
		void update();

		/* Funcao chamada pelo viewController para avisar o gameController que o usuario apertou uma tecla para
		 * ir para cima. Quando essa função for chamada, a posição do jogador deve ser atualizada no model
		 */
		void userPressedUp();

		/* Eqivalente a função anterior, mas agora eh a tecla para ir pra baixo
		 */
		void userPressedDown();
};

