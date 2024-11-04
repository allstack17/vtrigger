#include "triggerbot.h"

TriggerBot::TriggerBot(short zone) :
	_x(GetSystemMetrics(SM_CXSCREEN)), _y(GetSystemMetrics(SM_CYSCREEN)), _dots_distance(zone)
{
	_dots[LEFTUP_ANGLE]   = { (short)(_x / 2 - DIFF()), (short)(_y / 2 - DIFF()) };
	_dots[RIGHTDOWN_ANGE] = { (short)(_x / 2 + DIFF()), (short)(_y / 2 + DIFF()) };

	_hdc_info._hdc	    = GetDC(nullptr);
	_hdc_info._buff_hdc = CreateCompatibleDC(_hdc_info._hdc);
	_hdc_info._hmap	    = CreateCompatibleBitmap(_hdc_info._hdc, DIFF(), DIFF());
	SelectObject(_hdc_info._buff_hdc, _hdc_info._hmap);
}

TriggerBot::~TriggerBot()
{
	DeleteObject(_hdc_info._hmap);
	DeleteDC(_hdc_info._buff_hdc);
	ReleaseDC(nullptr, _hdc_info._hdc);
}

void TriggerBot::click(int button)
{
	INPUT input{};
	input.type 	 = INPUT_KEYBOARD;
	input.ki.wVk 	 = button;
	input.ki.dwFlags = 0;
	SendInput(1, &input, sizeof(INPUT));

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

bool TriggerBot::check_screen()
{
	static BITMAPINFO bmp_info{};
	bmp_info.bmiHeader.biSize 	 = sizeof(BITMAPINFO);
	bmp_info.bmiHeader.biWidth 	 = DIFF();
	bmp_info.bmiHeader.biHeight 	 = DIFF();
	bmp_info.bmiHeader.biPlanes 	 = 1;
	bmp_info.bmiHeader.biBitCount 	 = 32;
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

	/* purple */
	static uint8_t r = 250, g = 100, b = 250;

	/*
	 *	change the reaction to a certain color
	 */
	for (int i = 0; i < vec.size(); ++i) {
		if (( (GetRValue(vec[i]) - TOLERANCE < r) && (r < GetRValue(vec[i]) + TOLERANCE )) && 
			( (GetGValue(vec[i]) - TOLERANCE < g) && (g < GetRValue(vec[i]) + TOLERANCE )) &&
			( (GetBValue(vec[i]) - TOLERANCE < b) && (b < GetRValue(vec[i]) + TOLERANCE )) )
			return true;
	}
	return false;
}
