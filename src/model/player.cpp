#include "player.hpp"

int Player::get_pos_x(){
	return self.pos_x;
}

int Player::get_pos_y(){
	return self.pos_y;
}

void Player::move(int delta_x, int delta_y){
	self.pos_x += delta_x;
	self.pos_y += delta_y;
}

