#ifndef _MAIN_H_
#define _MAIN_H_

#include <iostream>
#include <windows.h>

#define ENEMY_COLOR
#define PX_DOTS_NUMBER 5

#define PX_INIT(x, y) (((x) / 2) + ((y) / 2))

/*	+ storage user screen resolution 
	+ px coords for read px data
*/
typedef struct ScreenCoordsOptions {
	int  _x;
	int  _y;
	int* _pixels;

public:
	ScreenCoordsOptions() : 
		_x(GetSystemMetrics(SM_CXSCREEN)), _y(GetSystemMetrics(SM_CYSCREEN)), _pixels(nullptr)
	{
		if (!_x || !_y)
			return;

		_pixels = new int[PX_DOTS_NUMBER] {};
		if (!_pixels)
			return;

		_pixels[0] = PX_INIT(_x, _y);
		_pixels[1] = PX_INIT(_x + 1, _y + 1);
		_pixels[2] = PX_INIT(_x + 1, _y - 1);
		_pixels[3] = PX_INIT(_x - 1, _y + 1);
		_pixels[4] = PX_INIT(_x - 1, _y - 1);

		std::cout << "px table initialized\n";

		for (int i = 0; i < PX_DOTS_NUMBER; ++i)
			std::cout << i << ": \t" << _pixels[i] << '\n';
	}

	ScreenCoordsOptions(const ScreenCoordsOptions& sco) :
		_x(sco._x), _y(sco._y), _pixels(new int[PX_DOTS_NUMBER])
	{
		if (_pixels)
			memcpy(_pixels, sco._pixels, PX_DOTS_NUMBER * sizeof(int));
	}

	~ScreenCoordsOptions() {
		delete[] _pixels;	
	}

public:
	void px_init();
} SCO;

void ScreenCoordsOptions::px_init()
{
	std::cout << "px init...\n"
		<< "hold [left alt] in the game window\n";

	while (GetAsyncKeyState(VK_MENU)) {

	}
}

#endif	// _MAIN_H_