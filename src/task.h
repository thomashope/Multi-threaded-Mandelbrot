#ifndef TASK_H
#define TASK_H

class Task
{
public:
	virtual ~Task() {}

	virtual void run() = 0;
private:
};

#endif