#include "triggerbot.h"

#define GET_COLOR(i, rgb, s) ((Get##rgb##Value(i)) s _pcfg->_tolerance)

#define RGB_COMPARE(i)\
	((GET_COLOR(i, R, -) < r) && (r < GET_COLOR(i, R, +)) &&  \
	 (GET_COLOR(i, G, -) < g) && (g < GET_COLOR(i, G, +)) &&  \
	 (GET_COLOR(i, B, -) < b) && (b < GET_COLOR(i, B, +)))	  \

TriggerBot::TriggerBot(const ConfigFileData* pcfg) :
	_x(GetSystemMetrics(SM_CXSCREEN)), _y(GetSystemMetrics(SM_CYSCREEN))
{
	_pcfg = const_cast<ConfigFileData*>(pcfg);

	_angle = { (short)(_x / 2 - _pcfg->_zone_x / 2), 
		   (short)(_y / 2 - _pcfg->_zone_y / 2) };

	_hdc_info._hdc	    = GetDC(nullptr);
	_hdc_info._buff_hdc = CreateCompatibleDC(_hdc_info._hdc);
	_hdc_info._hmap	    = CreateCompatibleBitmap(_hdc_info._hdc, _pcfg->_zone_x, _pcfg->_zone_y);

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
	static BITMAPINFO bmp_info{}; {
		bmp_info.bmiHeader.biSize 	 = sizeof(BITMAPINFO);
		bmp_info.bmiHeader.biWidth 	 = _pcfg->_zone_x;
		bmp_info.bmiHeader.biHeight 	 = -_pcfg->_zone_y;
		bmp_info.bmiHeader.biPlanes 	 = 1;
		bmp_info.bmiHeader.biBitCount 	 = 32;
		bmp_info.bmiHeader.biCompression = BI_RGB;
	}

	static std::vector<COLORREF> vec(_pcfg->_zone_x * _pcfg->_zone_y, 0);

	static const uint8_t r = 250, g = 100, b = 250;		/* purple */

	BitBlt(
		_hdc_info._buff_hdc,
		0, 0,
		_pcfg->_zone_x, _pcfg->_zone_y,
		_hdc_info._hdc,
		_angle.X, _angle.Y,
		SRCCOPY
	);

	GetDIBits(
		_hdc_info._buff_hdc,
		_hdc_info._hmap,
		0, _pcfg->_zone_y,
		vec.data(),
		&bmp_info,
		DIB_RGB_COLORS
	);
	
	for (const auto& i : vec) {
		if (RGB_COMPARE(i))
			return true;
	}

	return false;
}