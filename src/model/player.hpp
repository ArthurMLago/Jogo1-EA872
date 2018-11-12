#pragma once

class Player {
	private:
		float pos_x;
		int  pos_y;
		int dead;

		int socket_index;

	public:
		Player(float pos_x, int pos_y, int socket_index);
		~Player();

		float get_pos_x();
		int get_pos_y();
		int get_socket_index();
		void move(float delta_x, int delta_y);
		//void kill();
		//int is_dead();

};
