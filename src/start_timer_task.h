#ifndef START_TIMER_TASK_H
#define START_TIMER_TASK_H

#include <chrono>
#include "task.h"

using std::chrono::steady_clock;

// starts timing
class StartTimerTask : public Task
{
public:
	StartTimerTask(steady_clock::time_point* start) { start_ = start; }

	inline void run() override {
		*start_ = steady_clock::now();
	}

private:
	steady_clock::time_point* start_;
};

#endif