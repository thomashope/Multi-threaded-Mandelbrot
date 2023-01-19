#include "shuffle_lines_task.h"
#include <chrono>
#include <random>
#include <algorithm>

ShuffleLinesTask::ShuffleLinesTask(std::vector<MandelbrotLine*>* lines)
{
	mandelbrot_lines = lines;
}

void ShuffleLinesTask::run()
{
	// create a random number generator to shuffle the lines
	int seed = std::chrono::steady_clock::now().time_since_epoch().count();
	std::default_random_engine engine(seed);

	// lock all the lines
	// POTENTIAL DEADLOCK
	for( auto& l : *mandelbrot_lines ) {
		l->mutex_.lock();
	}

	std::shuffle(mandelbrot_lines->begin(), mandelbrot_lines->end(), engine);

	// unlock all the lines
	for( auto& l : *mandelbrot_lines ) {
		l->mutex_.unlock();
	}
}