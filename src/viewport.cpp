#include "viewport.h"


void move_viewport_left()
{
	double width = MbrotData.right - MbrotData.left;

	MbrotData.left -= width * 0.1;
	MbrotData.right -= width * 0.1;
}

void move_viewport_right()
{
	double width = MbrotData.right - MbrotData.left;

	MbrotData.left += width * 0.1;
	MbrotData.right += width * 0.1;
}

void move_viewport_up()
{
	double height = MbrotData.bottom - MbrotData.top;

	MbrotData.top -= height * 0.1;
	MbrotData.bottom -= height *  0.1;
}

void move_viewport_down()
{
	double height = MbrotData.bottom - MbrotData.top;

	MbrotData.top += height * 0.1;
	MbrotData.bottom += height *  0.1;
}

void zoom_viewport_in()
{
	double width = MbrotData.right - MbrotData.left;
	double height = MbrotData.bottom - MbrotData.top;

	MbrotData.left += width * 0.1;
	MbrotData.right -= width * 0.1;
	MbrotData.top += height * 0.1;
	MbrotData.bottom -= height * 0.1;
}

void zoom_viewport_out()
{
	double width = MbrotData.right - MbrotData.left;
	double height = MbrotData.bottom - MbrotData.top;

	MbrotData.left -= width * 0.1;
	MbrotData.right += width * 0.1;
	MbrotData.top -= height * 0.1;
	MbrotData.bottom += height * 0.1;
}

void reset_viewport()
{
	MbrotData.left = -2.0;
	MbrotData.right = 1.0;
	MbrotData.top = 1.25;
	MbrotData.bottom = -1.25;
}
