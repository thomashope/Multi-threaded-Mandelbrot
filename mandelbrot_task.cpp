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
			//line_->data_[x] = 0xff000000 | colour << 16 | colour << 8 | colour;
			
			// rotate the hue
			float h = (iterations > 255) ? iterations - 255 : iterations;
			Uint8 r, g, b;

			h = (h / 256.0f) * 6; // maps h to the range [0, 6)
			int i = std::floor(h); // i represent the setor of the colour wheel
			h -= i; // maps h to range [0, 1)
			switch( i )
			{
			case 0: // full red, no blue, increase green
				r = 255; b = 0;
				g = h * 255;
				break;
			case 1: // full green, no blue, decrease red
				g = 255; b = 0;
				r = (1 - h) * 255;
				break;
			case 2: // full greeen, no red, increase blue
				g = 255; r = 0;
				b = h * 255;
				break;
			case 3: // full blue, no red, decrease green
				b = 255; r = 0;
				g = (1 - h) * 255;
				break;
			case 4: // full blue, no green, increase red
				b = 255; g = 0;
				r = h * 255;
				break;
			case 5: // full red, no green, decrease blue
				r = 255; g = 0;
				b = (1 - h) * 255;
				break;
			default: r = g = b = 255;
				break;
			}

			line_->data_[x] = 0xff000000 | r << 16 | g << 8 | b;
		}
	}
}