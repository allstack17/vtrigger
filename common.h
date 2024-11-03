#ifndef _COMMON_H_
#define _COMMON_H_

#include <thread>
#include <chrono>
#include <windows.h>

#define sleep(ms)\
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));

#define _exit_msg(...) {\
	std::cerr << "msg:\t" << __VA_ARGS__ << "\nerror:\t" << GetLastError() << "\n\n";\
	system("pause");\
	exit(0);\
}

#endif	/* _COMMON_H_ */