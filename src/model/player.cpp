#include "player.hpp"

float Player::get_pos_x(){
	return this->pos_x;
}

float Player::get_pos_y(){
	return this->pos_y;
}

void Player::move(float delta_x, float delta_y){
	this->pos_x += delta_x;
	this->pos_y += delta_y;
}

