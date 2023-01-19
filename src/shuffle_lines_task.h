#ifndef SHUFFLE_LINES_TASK_H
#define SHUFFLE_LINES_TASK_H

#include <vector>
#include "mandelbrot_line.h"
#include "task.h"

// uses std::shuffle to shuffle all lines of the mandelbrot

class ShuffleLinesTask : public Task
{
public:
	ShuffleLinesTask(std::vector<MandelbrotLine*>* lines);

	void run() override;
private:

	std::vector<MandelbrotLine*>* mandelbrot_lines;
};

#endif