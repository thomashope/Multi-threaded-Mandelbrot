#ifndef CHECK_SORTED_TASK_H
#define CHECK_SORTED_TASK_H

#include <vector>
#include "mandelbrot_line.h"
#include "bubble_sort_task.h"
#include "stop_timer_task.h"
#include "farm.h"
#include "task.h"

// iteratos over the mandelbrot lines
// does if it is sorted
// if not then does adds another bubble sort task to the queue and checks again
class CheckSortedTask : public Task
{
public:
	CheckSortedTask(std::vector<MandelbrotLine*>* lines, Farm* farm, steady_clock::time_point* start);

	void run() override;

private:
	std::vector<MandelbrotLine*>* mandelbrot_lines_;

	Farm* farm_;

	bool sorted;

	// pass to a stop timer task when finished sorting
	steady_clock::time_point* start_;
};

#endif