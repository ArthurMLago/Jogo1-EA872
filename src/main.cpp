
#include "controller/gameController.hpp"

int main(){

	GameController gameController;
	ViewController viewController;
	Scene currentScene;


	viewController.setScene(&currentScene);
	viewController.setGameController(&gameController);

	gameController.setViewController(&viewController);
	gameController.setScene(&currentScene);
	
	
	int larg;
	int alt;
	viewController.getScreenDimension(&larg, &alt);

	currentScene.player = new Player(larg/2,alt/2);

	while(!gameController.shouldTerminate()){
	//while(1){
		gameController.update();
		viewController.drawScene();
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}


}
