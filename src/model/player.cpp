#include "player.hpp"

Player::Player(float pos_x,int pos_y){
	this->pos_x = pos_x;
	this->pos_y = pos_y;

}

float Player::get_pos_x(){
	return this->pos_x;
}

int Player::get_pos_y(){
	return this->pos_y;
}

void Player::move(float delta_x, int delta_y){
	this->pos_x += delta_x;
	this->pos_y += delta_y;
}

