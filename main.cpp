#include "main.h"

int main(int argc, char** argv)
{
	SCO* psco = new SCO;
	if (!psco) {
		std::cout << "failed to init SCO\n";
		return 0;
	}
	std::cout << "SCO initialized\n\n";

	psco->px_init();
	if (psco->_pixels) {
		std::cout << "failed to init px table\n";
		return 0;
	}
}