#include "triggerbot.h"

int main(int argc, char** argv)
{
	TriggerBot psi;

	while (1) {
		if (GetAsyncKeyState(VK_LMENU)) {
			if (psi.check_screen())
				psi.click('K');
		}
		Sleep(1);
	}
}