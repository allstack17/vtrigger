#ifndef _COMMON_H_
#define _COMMON_H_

#include <thread>
#include <atomic>
#include <chrono>
#include <windows.h>

#define WINDOW_X_SIZE		345
#define WINDOW_Y_SIZE		220

#define SC_COLOR_BLUE		1
#define SC_COLOR_GREEN		2
#define SC_COLOR_CYAN		3
#define SC_COLOR_RED		4
#define SC_COLOR_PURPLE		5
#define SC_COLOR_YELLOW		6
#define SC_COLOR_WHITE		7

#define __set_color(c) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c)

#define __set_color_exp(c, exp) {\
	__set_color(c); { exp; } __set_color(SC_COLOR_WHITE);\
}

#define __time_start(v)\
	auto v = std::chrono::high_resolution_clock::now()

#define __time_count(exp, cast)\
	std::chrono::duration_cast<std::chrono::cast>((exp)).count()

#define __sleep(ms)\
	std::this_thread::sleep_for(std::chrono::milliseconds(ms))

#define __exit_msg(...) {\
	__set_color_exp(SC_COLOR_RED,\
		std::cerr << "msg:\t" << __VA_ARGS__ << "\nerror:\t" << GetLastError() << "\n\n");\
	system("pause");\
	exit(0);\
}

#endif	/* _COMMON_H_ */