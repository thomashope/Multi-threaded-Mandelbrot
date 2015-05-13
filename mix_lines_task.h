#ifndef MIX_LINES_TASK_H
#define MIX_LINES_TASK_H

#include <vector>
#include <mutex>
#include "mandelbrot_task.h"

// takes two lines from the mandelbrot image and swaps them
class MixLinesTask : public Task
{
public:
	MixLinesTask(std::vector<MandelbrotLine*>* lines, std::mutex* epoch_mtx);

	void run() override;
private:

	std::mutex* epoch_mtx_;

	std::vector<MandelbrotLine*>* mandelbrot_lines;
};

#endif