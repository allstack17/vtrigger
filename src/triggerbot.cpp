/*
 * ----- TriggerBot -----
 * 
 * The trigger thread is started back in main() and through changing the global variable, the trigger is turned on/off.
 * The thread works with high priority.
 * 
 */

#include "triggerbot.h"
#include "common.h"

#define GET_COLOR(i, rgb, s) ((Get##rgb##Value(i)) s _pcfg->_tolerance)

#define RGB_COMPARE(i)\
	((GET_COLOR(i, R, -) < global::r) && (global::r < GET_COLOR(i, R, +)) &&  \
	 (GET_COLOR(i, G, -) < global::g) && (global::g < GET_COLOR(i, G, +)) &&  \
	 (GET_COLOR(i, B, -) < global::b) && (global::b < GET_COLOR(i, B, +)))	  \

namespace global {
	/* purple */
	const uint8_t r = 250, g = 100, b = 250;
}

/* global flag for trigger main loop */
bool TRIGGER_MAIN_LOOP = false;

TriggerBot::TriggerBot(ConfigFileData& pcfg) :
	_x(GetSystemMetrics(SM_CXSCREEN)), _y(GetSystemMetrics(SM_CYSCREEN))
{
	_pcfg				= &pcfg;
	_hdc_info._hdc	    = GetDC(nullptr);
	_hdc_info._buff_hdc = CreateCompatibleDC(_hdc_info._hdc);
	_hdc_info._hmap	    = CreateCompatibleBitmap(_hdc_info._hdc, _pcfg->_zone_x, _pcfg->_zone_y);

	SelectObject(_hdc_info._buff_hdc, _hdc_info._hmap);
}

TriggerBot::~TriggerBot() {
	DeleteObject(_hdc_info._hmap);
	DeleteDC(_hdc_info._buff_hdc);
	ReleaseDC(nullptr, _hdc_info._hdc);
}

void TriggerBot::click(int button) {
	INPUT input{};
	input.type 		 = INPUT_KEYBOARD;
	input.ki.wVk 	 = button;
	input.ki.dwFlags = 0;
	SendInput(1, &input, sizeof(INPUT));

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

bool TriggerBot::check_screen() {
	static BITMAPINFO bmp_info = {
		.bmiColors				 = {},
		.bmiHeader.biWidth		 = 0,
		.bmiHeader.biHeight		 = 0,
		.bmiHeader.biPlanes		 = 1,
		.bmiHeader.biBitCount	 = 32,
		.bmiHeader.biCompression = BI_RGB,
		.bmiHeader.biSize		 = sizeof(BITMAPINFO),
	};
	bmp_info.bmiHeader.biWidth  = _pcfg->_zone_x;
	bmp_info.bmiHeader.biHeight = _pcfg->_zone_y;

	static std::vector<COLORREF> vec;
	size_t old_size = vec.size();

	if (old_size != (_pcfg->_zone_x * _pcfg->_zone_y)) {
		old_size  = (_pcfg->_zone_x * _pcfg->_zone_y);
		vec.resize(old_size);
	}

	BitBlt(
		_hdc_info._buff_hdc, 0, 0,
		_pcfg->_zone_x, _pcfg->_zone_y, _hdc_info._hdc,
		(_x / 2 - _pcfg->_zone_x / 2), (_y / 2 - _pcfg->_zone_y / 2),
		SRCCOPY
	);

	GetDIBits(
		_hdc_info._buff_hdc, _hdc_info._hmap,
		0, _pcfg->_zone_y, vec.data(),
		&bmp_info, DIB_RGB_COLORS
	);
	
	for (const auto& i : vec) {
		if (RGB_COMPARE(i))
			return true;
	}

	return false;
}

void TriggerThread(ConfigFileData& cfg) {
	/* high prio for trigger thread */
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	TriggerBot bot(cfg);

	while (1) {
		if (TRIGGER_MAIN_LOOP && GetAsyncKeyState(cfg._hotkey)) {
			if (bot.check_screen()) {
				bot.click(cfg._second_key);
				__sleep(cfg._delay);
			}
		}
		__sleep(1);
	}
}
