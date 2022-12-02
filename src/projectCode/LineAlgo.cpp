#include "LineAlgo.hpp"

std::vector<glm::vec2> DrawLine(int x1, int y1, int x2, int y2)
{
	std::vector<glm::vec2> pixels;

	int dx, dy, Po;
	int k = 0;
	dx = (x2 - x1);
	dy = (y2 - y1);
	if (dy <= dx && dy > 0)
	{
		dx = abs(dx);
		dy = abs(dy);
		Po = (2 * dy) - dx;
		pixels.push_back(glm::vec2(x1, y1));
		int xk = x1;
		int yk = y1;
		for (k = x1; k < x2; k++)
		{
			if (Po < 0)
			{
				pixels.push_back(glm::vec2(++xk, yk));
				Po = Po + (2 * dy);
			}
			else {
				pixels.push_back(glm::vec2(++xk, ++yk));
				Po = Po + (2 * dy) - (2 * dx);
			}
		}
	}
	else if (dy > dx && dy > 0)
	{
		dx = abs(dx);
		dy = abs(dy);
		Po = (2 * dx) - dy;
		pixels.push_back(glm::vec2(x1, y1));
		int xk = x1; int yk = y1;
		for (k = y1; k < y2; k++)
		{
			if (Po < 0)
			{
				pixels.push_back(glm::vec2(xk, ++yk));
				Po = Po + (2 * dx);
			}
			else {
				pixels.push_back(glm::vec2(++xk, ++yk));
				Po = Po + (2 * dx) - (2 * dy);
			}
		}
	}
	else if (dy >= -dx)
	{
		dx = abs(dx);
		dy = abs(dy);
		Po = (2 * dy) - dx;
		pixels.push_back(glm::vec2(x1, y1));
		int xk = x1;
		int yk = y1;
		for (k = x1; k < x2; k++)
		{
			if (Po < 0)
			{
				pixels.push_back(glm::vec2(++xk, yk));
				Po = Po + (2 * dy);
			}
			else {
				pixels.push_back(glm::vec2(++xk, --yk));
				Po = Po + (2 * dy) - (2 * dx);
			}
		}
	}
	else if (dy < -dx)
	{
		dx = abs(dx);
		dy = abs(dy);
		Po = (2 * dy) - dx;
		pixels.push_back(glm::vec2(x1, y1));
		int xk = x1;
		int yk = y1;
		for (k = y1; k > y2; k--)
		{
			if (Po < 0)
			{
				pixels.push_back(glm::vec2(xk, --yk));
				Po = Po + (2 * dx);
			}
			else {
				pixels.push_back(glm::vec2(++xk, --yk));
				Po = Po + (2 * dx) - (2 * dy);
			}
		}
	}

	return pixels;
}

