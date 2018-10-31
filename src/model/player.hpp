#pragma once

class Player {
        private:
                float pos_x;
                int  pos_y;

        public:
                Player(float pos_x, int pos_y);

                float get_pos_x();
                int get_pos_y();
                void move(float delta_x,int delta_y);

};
                                                                               
                                                                        
