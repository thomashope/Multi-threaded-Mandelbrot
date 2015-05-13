#ifndef MANDELBROT_TASK_H
#define MANDELBROT_TASK_H

#include "task.h"
#include <complex>

// forward declarations
class MandelbrotLine;

// global width/height of the window
extern const int WIDTH;
extern const int HEIGHT;
extern const int MAX_ITERATIONS;


class MandelbrotTask : public Task
{
public:
	MandelbrotTask(MandelbrotLine* line);

	void run() override;

private:
	MandelbrotLine* line_;

	inline double length_squared(std::complex<double> complex) {
		return complex.real() * complex.real() + complex.imag() * complex.imag();
	}
};

#endif