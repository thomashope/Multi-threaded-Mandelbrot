#ifndef BUBBLE_SORT_TASK_H
#define BUBBLE_SORT_TASK_H

#include <vector>
#include "mandelbrot_line.h"
#include "task.h"

// applys bubble sort to the lines of the mandelbrot to get them back in order
// each bubble sort task does only one sweep
class BubbleSortTask : public Task
{
public:
	BubbleSortTask(std::vector<MandelbrotLine*>* lines);

	void run() override;

private:
	size_t bubbleIndex{ 0 };

	std::vector<MandelbrotLine*>* mandelbrot_lines;

	bool increment();

	void swap();
};

#endif