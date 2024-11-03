#ifndef _TRIGGER_BOT_H_
#define _TRIGGER_BOT_H_

#include <iostream>
#include <vector>
#include <windows.h>

#define _exit_msg(...) {\
	std::cerr << "msg:\t" << __VA_ARGS__ << "\nerror:\t" << GetLastError() << '\n';\
}

/* triggerbot zone reaction */
#define DOTS_DISTANCE	7

/* for TriggerBot::_dots */
#define LEFTUP_ANGLE	0
#define RIGHTDOWN_ANGE	1

#define DIFF()(DOTS_DISTANCE * 2)

struct TriggerBot
{
	short _x, _y;
	COORD _dots[2];

private:
	struct HdcInfo {
		HDC		_hdc, _buff_hdc;
		HBITMAP _hmap;
	} _hdc_info{};

public:
	TriggerBot();

	~TriggerBot();

public:
	void click(int);

	bool check_screen();
};

TriggerBot::TriggerBot() : 
	_x(GetSystemMetrics(SM_CXSCREEN)), _y(GetSystemMetrics(SM_CYSCREEN))
{
	_dots[LEFTUP_ANGLE]   = { (short)(_x / 2 - DOTS_DISTANCE), (short)(_y / 2 - DOTS_DISTANCE) };
	_dots[RIGHTDOWN_ANGE] = { (short)(_x / 2 + DOTS_DISTANCE), (short)(_y / 2 + DOTS_DISTANCE) };

	_hdc_info._hdc		 = GetDC(nullptr);
	_hdc_info._buff_hdc	 = CreateCompatibleDC(_hdc_info._hdc);
	_hdc_info._hmap		 = CreateCompatibleBitmap(_hdc_info._hdc, DIFF(), DIFF());
	SelectObject(_hdc_info._buff_hdc, _hdc_info._hmap);
}

TriggerBot::~TriggerBot()
{
	DeleteObject(_hdc_info._hmap);
	DeleteDC	(_hdc_info._buff_hdc);
	ReleaseDC	(nullptr, _hdc_info._hdc);
}

void TriggerBot::click(int button)
{
	INPUT input{};
	input.type		 = INPUT_KEYBOARD;
	input.ki.wVk	 = button;
	input.ki.dwFlags = 0;
	SendInput(1, &input, sizeof(INPUT));

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

bool TriggerBot::check_screen()
{
	static BITMAPINFO bmp_info{};
	bmp_info.bmiHeader.biSize		 = sizeof(BITMAPINFO);
	bmp_info.bmiHeader.biWidth		 = DIFF();
	bmp_info.bmiHeader.biHeight		 = DIFF();
	bmp_info.bmiHeader.biPlanes		 = 1;
	bmp_info.bmiHeader.biBitCount	 = 32;
	bmp_info.bmiHeader.biCompression = BI_RGB;

	BitBlt(
		_hdc_info._buff_hdc,
		0, 0,
		DIFF(), DIFF(),
		_hdc_info._hdc,
		_dots[LEFTUP_ANGLE].X, _dots[LEFTUP_ANGLE].Y,
		SRCCOPY
	);

	static std::vector<COLORREF> vec(DIFF() * DIFF(), 0);

	GetDIBits(
		_hdc_info._buff_hdc,
		_hdc_info._hmap,
		0, DIFF(),
		vec.data(),
		&bmp_info,
		DIB_RGB_COLORS
	);

	/*  
	 *	change the reaction to a certain color
	 *	current: purple
	 */
	for (int i = 0; i < vec.size(); ++i) {
		int tmp = RGB((int)GetRValue(vec[i]), (int)GetGValue(vec[i]), (int)GetBValue(vec[i]));
		if (tmp >= 0xff38ff && tmp <= 0xff8aff)
			return true;
	}

	return false;
}

#endif	/* _TRIGGER_BOT_H_ */