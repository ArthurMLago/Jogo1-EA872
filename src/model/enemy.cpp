#include "enemy.hpp"

Enemy::Enemy(float pos_x, int pos_y, float speed){
	this->pos_x = pos_x;
	this->pos_y = pos_y;
	this->speed = speed;
}


float Enemy::get_pos_x(){
	return this->pos_x;
}

int Enemy::get_pos_y(){
	return this->pos_y;
}

float Enemy::get_speed(){
	return this->speed;
}

void Enemy::move(float delta_x, int delta_y){
	this->pos_x += delta_x;
	this->pos_y += delta_y;
}

