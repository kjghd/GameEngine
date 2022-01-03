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

public:
	void SetUnchanged()
	{
		for (auto& button : aButtons)
			button.changed = false;
	}

	void SetButtonsOn(WPARAM wParam)
	{
		switch (wParam)
		{
		case 0x41: SetOn(BID_A); break;
		case 0x44: SetOn(BID_D); break;
		case 0x53: SetOn(BID_S); break;
		case 0x57: SetOn(BID_W); break;
		}
	}
	void SetButtonsOff(WPARAM wParam)
	{
		switch (wParam)
		{
		case 0x41: SetOff(BID_A); break;
		case 0x44: SetOff(BID_D); break;
		case 0x53: SetOff(BID_S); break;
		case 0x57: SetOff(BID_W); break;
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