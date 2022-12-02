#include "../projectCode/Screen.hpp"


int main()
{
	Screen screen(200, 200);

	int pos = 5;

	bool running = true;
	while (running)
	{
		// resetting screen and buffer
		screen.ClearScreen();
		screen.ClearBuffer();

		// do game logic here
		screen.PlotPixel(glm::vec2(pos, pos));
		screen.PlotPixel(glm::vec2(pos-1, pos));
		screen.PlotPixel(glm::vec2(pos-1, pos-1));
		screen.PlotPixel(glm::vec2(pos, pos-1));
		
		// drawing
		screen.OutputBuffer();
	}
}