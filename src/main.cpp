
#include "controller/gameController.hpp"

int main(){

	GameController gameController;
	ViewController viewController;
	Scene currentScene;

	Enemy *newEnemy = new Enemy(10.0, 10.0, 5.0);
	currentScene.player = new Player(50,50);
	currentScene.enemyList.push_back(newEnemy);

	viewController.setScene(&currentScene);

	while(1){
		gameController.update();
		viewController.drawScene();
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}


}
