#include "check_sorted_task.h"

CheckSortedTask::CheckSortedTask(std::vector<MandelbrotLine*>* lines, Farm* farm, steady_clock::time_point* start)
{
	mandelbrot_lines_ = lines;

	farm_ = farm;

	sorted = true;

	start_ = start;
}

void CheckSortedTask::run()
{
	MandelbrotLine* previous{ (*mandelbrot_lines_)[0] };

	// iterate over the lines and check order
	for( auto line : *mandelbrot_lines_ )
	{
		if( previous->line_y_ > line->line_y_ ) {
			sorted = false;
			break;
		}

		previous = line;
	}

	if( !sorted )
	{
		farm_->add_task(new BubbleSortTask(mandelbrot_lines_));
		farm_->add_task(new CheckSortedTask(mandelbrot_lines_, farm_, start_));
	}
	else
	{
		farm_->add_task(new StopTimerTask(start_));
	}

}