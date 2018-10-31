#pragma once

class UserResponder{
	public:
		virtual void userPressedUp() = 0;
		virtual void userPressedDown() = 0;
		virtual void userPressedLeft() = 0;
		virtual void userPressedRight() = 0;

		virtual void terminate() = 0;
		virtual int shouldTerminate() = 0;



};
