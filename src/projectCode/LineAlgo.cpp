#include "LineAlgo.hpp"

std::vector<glm::vec2> DrawLineBrensenham(int x1, int b, int x2, int y2) // I ripped this straight off github idk how it works
{
	std::vector<glm::vec2> points;

	int dx, dy, i, e;
	int incx, incy, inc1, inc2;
	int x, y;

	dx = x2 - x1;
	dy = y2 - b;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	incx = 1;
	if (x2 < x1)
		incx = -1;
	incy = 1;
	if (y2 < b)
		incy = -1;
	x = x1;
	y = b;
	if (dx > dy)
	{
		points.push_back(glm::vec2(x, y));
		e = 2 * dy - dx;
		inc1 = 2 * (dy - dx);
		inc2 = 2 * dy;
		for (i = 0; i < dx; i++)
		{
			if (e >= 0)
			{
				y += incy;
				points.push_back(glm::vec2(x, y));
				e += inc1;
				x += incx;
			}
			else
			{
				e += inc2; x += incx;
				//points.push_back(glm::vec2(x, y - incy));
			}
			points.push_back(glm::vec2(x, y));
		}
	}
	else
	{
		points.push_back(glm::vec2(x, y));
		e = 2 * dx - dy;
		inc1 = 2 * (dx - dy);
		inc2 = 2 * dx;
		for (i = 0; i < dy; i++)
		{
			if (e >= 0)
			{
				x += incx;
				points.push_back(glm::vec2(x, y));
				y += incy;
				e += inc1;

			}
			else
			{
				e += inc2; y += incy;
				//points.push_back(glm::vec2(x + incx, y));
			}

			points.push_back(glm::vec2(x, y));
		}
	}

	return points;
}

