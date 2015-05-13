#include "mandelbrot_task.h"
#include "mandelbrot_line.h"
#include <iostream>

MandelbrotTask::MandelbrotTask(MandelbrotLine* line)
{
	line_ = line;
}

void MandelbrotTask::run()
{
	// compute a line of the mandelbrot set

	int y = line_->line_y_;
	double left   = MbrotData.left;
	double right  = MbrotData.right;
	double top    = MbrotData.top;
	double bottom = MbrotData.bottom;
	
	for( int x = 0; x < WIDTH; ++x )
	{
		// Work out the point in the complex plane that
		// corresponds to this pixel in the output image.
		std::complex<double> c(left + (x * (right - left) / WIDTH),
			top + (y * (bottom - top) / HEIGHT));

		// Start off z at (0, 0).
		std::complex<double> z(0.0, 0.0);

		// Iterate z = z^2 + c until z moves more than 2 units
		// away from (0, 0), or we've iterated too many times.
		unsigned int iterations = 0;

		// use the orbit reference to check if z is repeating itself
		std::complex<double> orbit_reference = z;
		unsigned int orbit_counter = 16;

		while( length_squared(z) < 4.0 && iterations < MAX_ITERATIONS )
		{
			//*
			z = (z * z) + c;			

			++iterations;

			if( orbit_reference == z ) { iterations = MAX_ITERATIONS;  break; }

			if( orbit_counter == iterations )
			{
				orbit_reference = z;

				orbit_counter += 8;
			}

			//*/

			/*
			z = (z * z) + c;
			++iterations;
			//*/
		}

		if( iterations >= MAX_ITERATIONS )
		{
			// z didn't escape from the circle.
			// This point is in the Mandelbrot set.

			std::unique_lock<std::mutex> line_lock(line_->mutex_);

			// black
			line_->data_[x] = 0xff000000;
		}
		else 
		{
			Uint8 colour = iterations >> 1;

			std::unique_lock<std::mutex> line_lock(line_->mutex_);

			// write data as ARGB
			line_->data_[x] = 0xff000000 | colour << 16 | colour << 8 | colour;
		}
	}
}