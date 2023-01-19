#ifndef MESSAGE_TASK_H
#define MESSAGE_TASK_H

#include <iostream>
#include <string>
#include <mutex>
#include "task.h"

class MessageTask : public Task
{
public:
	MessageTask(std::string s, std::mutex* m) : message_(s), print_mutex_(m) {}

	inline void run() override {
		print_mutex_->lock();
		std::cout << message_ << std::endl;
		print_mutex_->unlock();
	}

private:
	std::string message_;
	std::mutex* print_mutex_;
};

#endif