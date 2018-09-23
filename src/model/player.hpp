#pragma once

class Player {
	private:
		float pos_x;
		float pos_y;

	public:
		float get_pos_x();
		float get_pos_y();
		void move(float delta_x, float delta_y);

};
