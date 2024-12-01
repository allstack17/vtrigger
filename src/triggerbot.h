#ifndef _TRIGGER_BOT_H_
#define _TRIGGER_BOT_H_

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

struct ConfigFileData;

class TriggerBot 
{
	short _x, _y;
	ConfigFileData* _pcfg;

private:
	struct HdcInfo {
		HDC	_hdc, _buff_hdc;
		HBITMAP _hmap;
	} _hdc_info{};

public:
	TriggerBot(ConfigFileData&);

	~TriggerBot();

public:
	void click(int);

	bool check_screen();
};

struct ConfigFileData 
{
	uint8_t _hotkey, _second_key, _zone_x, _zone_y, _tolerance;
	int _delay;

public:
	ConfigFileData() {
		_hotkey = _second_key = _delay =
			_tolerance = _zone_x = _zone_y = 0;
	}
};

void TriggerThread(ConfigFileData&);
void SetConfigFileData(const std::string&, const int&) noexcept;
ConfigFileData* ReadConfigFile() noexcept;

#endif	/* _TRIGGER_BOT_H_ */