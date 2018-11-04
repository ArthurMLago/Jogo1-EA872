#pragma once

class Player {
	private:
		float pos_x;
		int  pos_y;

		int socket_index;

	public:
		Player(float pos_x, int pos_y, int socket_index);

		float get_pos_x();
		int get_pos_y();
		void move(float delta_x, int delta_y);

};
                                                                               
                                                                        
