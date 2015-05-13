#ifndef MANDELBROT_LINE_H
#define MANDELBROT_LINE_H

#include <SDL.h>
#include <mutex>
#include <atomic>

// describes which section of the mandelbrot should be drawn
struct MandelbrotData {
	double left{ -2.0 };
	double right{ 1.0 };
	double top{ 1.25 };
	double bottom{ -1.25 };
};

// global values for calculating the mandelbrot
extern MandelbrotData MbrotData;

extern const int WIDTH;

class MandelbrotLine
{
public:
	MandelbrotLine(int height);
	~MandelbrotLine();

	// draws the line of pixels at y position
	void draw(SDL_Texture* ren, std::mutex* screen_mutex_, int position);

	// an array of pixel data for the line
	// ARGB format
	Uint32* data_;
	
	// the y position of the line in the final image
	// 0 at the top of the screen
	int line_y_;

	std::mutex mutex_;
};

#endif