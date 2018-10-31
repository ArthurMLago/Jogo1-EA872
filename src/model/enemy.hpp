#pragma once

class Enemy{
	private:
		float pos_x;
		int pos_y;
		float speed;
	public:
		Enemy(float pos_x, int pos_y, float speed);

		float get_pos_x();
		int get_pos_y();
		float get_speed();

		void move(float delta_x, int delta_y);
};

