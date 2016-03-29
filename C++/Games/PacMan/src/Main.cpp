#include "Graphics.h"
#include "Controls.h"
#include "Constants.h"

int main(int argc, char* argv[])
{
	srand(RandomSeed);
	if (!initGraphics()) {
		return 1;
	}

	Fps fps(FPS);

	board.initTimers();
	while (isRunning)
	{
		handleInput();
		board.update();
		clearScreen();
		render();
		fps.limit();
	}

	exitGame(0);
	return 0;
}