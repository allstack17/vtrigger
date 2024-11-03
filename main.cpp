#include "triggerbot.h"
#include "common.h"

int main(int argc, char** argv)
{
	MoveWindow(GetConsoleWindow(), 500, 500, 500, 400, true);
	SetWindowTextA(GetConsoleWindow(), "triggerbot");

	ConfigFileData* cfg = ReadConfigFile();

	register TriggerBot psi(cfg->_dots_distance);

	std::cout << "\nrunning...\n";
	while (1) {
		if (cfg->_always_enable) {
			if (psi.check_screen()) {
				psi.click(cfg->_second_key);
				sleep(cfg->_delay);
			}
		}
		else {
			if (GetAsyncKeyState(cfg->_hotkey)) {
				if (psi.check_screen()) {
					psi.click(cfg->_second_key);
					sleep(cfg->_delay);
				}
			}
		}
		sleep(1);
	}
}