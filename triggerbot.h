#ifndef _TRIGGER_BOT_H_
#define _TRIGGER_BOT_H_

#include <iostream>
#include <vector>
#include <windows.h>

/* triggerbot zone reaction */
#define DOTS_DISTANCE	4

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

struct ConfigFileData
{
	uint8_t _hotkey, _second_key;
	int		_delay;
	bool	_always_enable;

public:
	ConfigFileData() :
		_hotkey(0), _second_key(0), _delay(0), _always_enable(false) {}
};

ConfigFileData* ReadConfigFile();

#endif	/* _TRIGGER_BOT_H_ */