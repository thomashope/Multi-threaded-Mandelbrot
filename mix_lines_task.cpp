#include "mix_lines_task.h"
#include "mandelbrot_line.h"
#include <mutex>
#include <thread>
#include <chrono>
#include <random>

MixLinesTask::MixLinesTask(std::vector<MandelbrotLine*>* lines, std::mutex* epoch_mtx)
{
	mandelbrot_lines = lines;

	epoch_mtx_ = epoch_mtx;
}

void MixLinesTask::run()
{	
	int first, second, seed;

	// prevent multiple threads from using the same seed (time itself)
	epoch_mtx_->lock();
	{
		seed = std::chrono::steady_clock::now().time_since_epoch().count();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	epoch_mtx_->unlock();

	// create the random engine using unique seed
	std::default_random_engine engine(seed);
	std::uniform_int_distribution<int> dist(0, mandelbrot_lines->size()-1);

	// select two different lines
	do 
	{
		first = dist(engine);
		second = dist(engine);
	} while( first == second );

	// take controll of both lines
	std::lock(
		(*mandelbrot_lines)[first]->mutex_,
		(*mandelbrot_lines)[second]->mutex_
		);

	// swap their contents
	std::swap(
		(*mandelbrot_lines)[first],
		(*mandelbrot_lines)[second]
		);

	// release both lines
	(*mandelbrot_lines)[first]->mutex_.unlock();
	(*mandelbrot_lines)[second]->mutex_.unlock();
}