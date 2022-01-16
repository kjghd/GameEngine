#pragma once
#include <array>
#include <sstream>
#include <d2d1.h>


enum eButtonID
{
	BID_LMB,
	BID_RMB,

	BID_CTRL_L,

	BID_A,
	BID_D,
	BID_S,
	BID_W,

	BID_BRACKET_SQR_L,
	BID_BRACKET_SQR_R,

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
	D2D1_POINT_2F mouse;

	void SetUnchanged()
	{
		for (auto& button : aButtons)
			button.changed = false;
	}

	void SetMouseDown()
	{
		if (!aButtons.at(BID_LMB).on)
		{
			aButtons.at(BID_LMB).on = true;
			aButtons.at(BID_LMB).changed = true;
		}
	}
	void SetMouseUp()
	{
		aButtons.at(BID_LMB).on = false;
		aButtons.at(BID_LMB).changed = true;
	}
	void UpdateMousePosition(LPARAM lParam)
	{
		// Incorrect on multiple monitors.
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);
	}

	void SetButtonsOn(WPARAM wParam)
	{
		switch (wParam)
		{
		case VK_CONTROL: SetOn(BID_CTRL_L);		   break;
		case 0x41:		 SetOn(BID_A);			   break;
		case 0x44:		 SetOn(BID_D);			   break;
		case 0x53:		 SetOn(BID_S);			   break;
		case 0x57:		 SetOn(BID_W);			   break;
		case VK_OEM_4:	 SetOn(BID_BRACKET_SQR_L); break;
		case VK_OEM_6:	 SetOn(BID_BRACKET_SQR_R); break;
		}
	}
	void SetButtonsOff(WPARAM wParam)
	{
		switch (wParam)
		{
		case VK_CONTROL: SetOff(BID_CTRL_L);		break;
		case 0x41:		 SetOff(BID_A);				break;
		case 0x44:		 SetOff(BID_D);				break;
		case 0x53:		 SetOff(BID_S);				break;
		case 0x57:		 SetOff(BID_W);				break;
		case VK_OEM_4:	 SetOff(BID_BRACKET_SQR_L); break;
		case VK_OEM_6:	 SetOff(BID_BRACKET_SQR_R); break;
		}
	}

	bool CheckPressed(int buttonID)
	{
		return aButtons.at(buttonID).on && aButtons.at(buttonID).changed;
	}
	bool CheckHeld(int buttonID)
	{
		return aButtons.at(buttonID).on && !aButtons.at(buttonID).changed;
	}

	bool CheckReleased(int buttonID)
	{
		return !aButtons.at(buttonID).on && aButtons.at(buttonID).changed;
	}

};