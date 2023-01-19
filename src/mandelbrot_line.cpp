#include "mandelbrot_line.h"

MandelbrotLine::MandelbrotLine(int height)
{
	line_y_ = height;

	data_ = new Uint32[WIDTH]();
}

MandelbrotLine::~MandelbrotLine()
{
	delete[] data_;
	data_ = nullptr;
}

void MandelbrotLine::draw(SDL_Texture* tex, std::mutex* texture_mutex_, int position)
{
	texture_mutex_->lock();

	SDL_Rect draw_area{ 0, position, WIDTH, 1};

	SDL_UpdateTexture(tex, &draw_area, data_, sizeof(Uint32) * WIDTH);

	texture_mutex_->unlock();
}