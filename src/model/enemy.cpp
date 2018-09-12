#include "enemy.hpp"

Enemy::Enemy(int pos_x, int pos_y, float speed){
	this->pos_x = pos_x;
	this->pos_y = pos_y;
	this->speed = speed;
}


int Enemy::get_pos_x(){
	return this->pos_x;
}

int Enemy::get_pos_y(){
	return this->pos_x;
}

float Enemy::get_speed(){
	return this->speed;
}

void Enemy::move(int delta_x, int delta_y){
	this->pos_x += delta_x;
	this->pos_y += delta_y;
}





//update:AEIBUQGHUQ9UEGHGQEU0H
//lastTime = nowTIme
//noit = get timweg
//RFNABER
// bolinha.move(bolinha.get_speed * tempo_que_passou ,0)
