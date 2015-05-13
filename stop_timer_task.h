#ifndef STOP_TIMER_TASK_H
#define STOP_TIMER_TASK_H

#include <iostream>
#include <chrono>
#include "task.h"

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::cout;
using std::endl;

// prints the elapesed time
class StopTimerTask : public Task
{
public:
	StopTimerTask(steady_clock::time_point* start) { start_ = start; }

	inline void run() override
	{
		// mark the end time point
		auto end = steady_clock::now();

		// calculate the difference
		auto time_elapsed = duration_cast<milliseconds>(end - *start_);

		cout << "Time elapsed in milliseconds: " << time_elapsed.count() << endl;
	}

private:
	steady_clock::time_point* start_;
};

#endif