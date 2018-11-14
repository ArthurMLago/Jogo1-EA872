#pragma once

class Player {
	private:
		float pos_x;
		int  pos_y;

		int socket_index;
        char color;

	public:
		Player(float pos_x, int pos_y, int socket_index);
		~Player();

		float get_pos_x();
		int get_pos_y();
		int get_socket_index();
		void move(float delta_x, int delta_y);

};
