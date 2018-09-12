#pragma once

class Player {
	private:
		int pos_x;
		int pos_y;

	public:
		int get_pos_x();
		int get_pos_y();
		void move(int delta_x, int delta_y);

};
