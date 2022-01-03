#pragma once
#include <array>
#include <sstream>


enum eButtonID
{
	BID_A,
	BID_D,
	BID_S,
	BID_W,

	BID_COUNT
};

class Input
{
	struct Button
	{
		bool on{ false };
		bool changed{ false };
	};
	std::array<Button, BID_COUNT> aButtons;

public:
	void SetUnchanged()
	{
		for (auto& button : aButtons)
			button.changed = false;
	}

	void SetOn(int buttonID)
	{
		if (!aButtons.at(buttonID).on)
		{
			aButtons.at(buttonID).on = true;
			aButtons.at(buttonID).changed = true;
		}
	}
	void SetOff(int buttonID)
	{
		if (aButtons.at(buttonID).on)
		{
			aButtons.at(buttonID).on = false;
			aButtons.at(buttonID).changed = true;
		}
	}



	bool CheckPressed(int buttonID)
	{
		return aButtons.at(buttonID).on && aButtons.at(buttonID).changed;
	}
	bool CheckReleased(int buttonID)
	{
		return !aButtons.at(buttonID).on && aButtons.at(buttonID).changed;
	}

};