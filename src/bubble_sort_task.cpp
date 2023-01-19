#include "bubble_sort_task.h"
#include <mutex>

BubbleSortTask::BubbleSortTask(std::vector<MandelbrotLine*>* lines)
{
	mandelbrot_lines = lines;
}


void BubbleSortTask::run()
{
	do{
		// take controll of both lines
		std::lock(
			(*mandelbrot_lines)[bubbleIndex]->mutex_,
			(*mandelbrot_lines)[bubbleIndex + 1]->mutex_
			);

		// if the lines are in the wrong order
		if( (*mandelbrot_lines)[bubbleIndex]->line_y_ >
			(*mandelbrot_lines)[bubbleIndex + 1]->line_y_ )
		{
			swap();
		}

		// release both lines
		(*mandelbrot_lines)[bubbleIndex]->mutex_.unlock();
		(*mandelbrot_lines)[bubbleIndex + 1]->mutex_.unlock();
		
	} while( increment() );
}

bool BubbleSortTask::increment()
{
	if( bubbleIndex < mandelbrot_lines->size() - 2 ) {
		bubbleIndex++;
		return true;
	} else {
		return false;
	}
}

void BubbleSortTask::swap()
{
	std::swap(
		(*mandelbrot_lines)[bubbleIndex]->data_,
		(*mandelbrot_lines)[bubbleIndex+1]->data_
		);

	std::swap(
		(*mandelbrot_lines)[bubbleIndex]->line_y_,
		(*mandelbrot_lines)[bubbleIndex+1]->line_y_
		);

}