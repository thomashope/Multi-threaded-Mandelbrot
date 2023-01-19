#ifndef FARM_H
#define FARM_H

#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
#include "checkpoint.h"

// forward declarations
class Task;

class Farm
{
public:
	Farm() { finished_work_ = false; }
	~Farm() {}

	// give a task to the worker to be completed
	void add_task(Task* t);

	// removes all tasks from the queue
	void clear_tasks();

	// returns true if the farm still has tasks to complete
	bool is_done() const;

	void create_workers(int num_workers);

	// tells the worker to stop and join
	void join_workers();

private:
	// holds pointers to the worker threads
	std::vector<std::thread*> threads_;

	// holds all tasks yet to be executed
	std::queue<Task*> tasks_;

	// prevents multiple workers from pulling the same task from the queue
	std::mutex task_queue_mutex_;

	// prevents workers from pulling from the queue when it is being cleared
	Checkpoint clear_queue_checkpoint_;
	
	// creates a new single thread and kicks it off executing tasks
	void create_worker();

	// execuset tasks in the queue
	void do_work();

	// true when the worker should quit
	std::atomic_bool finished_work_;
};

#endif