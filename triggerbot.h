#ifndef _TRIGGER_BOT_H_
#define _TRIGGER_BOT_H_

#include <iostream>
#include <vector>
#include <windows.h>

#define TOLERANCE 	20

/* for TriggerBot::_dots */
#define LEFTUP_ANGLE	0
#define RIGHTDOWN_ANGE	1

#define DIFF()(_dots_distance * 2)

struct TriggerBot
{
	short _x, _y, _dots_distance;
	COORD _dots[2];

private:
	struct HdcInfo {
		HDC	_hdc, _buff_hdc;
		HBITMAP _hmap;
	} _hdc_info{};

public:
	TriggerBot(short);

	~TriggerBot();

public:
	void click(int);

	bool check_screen();
};

struct ConfigFileData
{
	uint8_t _hotkey, _second_key;
	int	_delay;
	bool	_always_enable;
	short	_dots_distance;

public:
	ConfigFileData() :
		_hotkey(0), _second_key(0), _delay(0), _always_enable(false), _dots_distance(0) {}
};

ConfigFileData* ReadConfigFile();

#endif	/* _TRIGGER_BOT_H_ */
