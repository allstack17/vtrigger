#ifndef _TRIGGER_BOT_H_
#define _TRIGGER_BOT_H_

#include <iostream>
#include <vector>
#include <windows.h>

#define __SIMD
#ifdef  __SIMD
#include <x86intrin.h>
#endif

struct ConfigFileData;

struct TriggerBot
{
	short _x, _y;
	COORD _angle;
	ConfigFileData* _pcfg;

private:
	struct HdcInfo {
		HDC	_hdc, _buff_hdc;
		HBITMAP _hmap;
	} _hdc_info{};

public:
	TriggerBot(const ConfigFileData*);

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
	short	_tolerance;
	short	_zone_x, _zone_y;

public:
	ConfigFileData() {
		_hotkey = _second_key = _delay = _always_enable
			= _tolerance = _zone_x = _zone_y = 0;
	}
};

ConfigFileData* ReadConfigFile() noexcept;

#endif	/* _TRIGGER_BOT_H_ */