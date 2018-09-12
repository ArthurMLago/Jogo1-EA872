#pragma once

class Enemy{
	private:
		int pos_x;
		int pos_y;
		float speed;
	public:
		Enemy(int pos_x, int pos_y, float speed);

		int get_pos_x();
		int get_pos_y();
		float get_speed();

		void move(int delta_x, int delta_y);
};

