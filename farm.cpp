#include "farm.h"
#include "task.h"
#include <chrono>
#include <thread>
#include <functional>

void Farm::add_task(Task* t)
{
	clear_queue_checkpoint_.close();

	task_queue_mutex_.lock();
	{
		// add the task to the queue
		tasks_.push(t);
	}
	task_queue_mutex_.unlock();

	clear_queue_checkpoint_.open();
}

void Farm::clear_tasks()
{
	// stop threads from trying to access the queue
	clear_queue_checkpoint_.close();

	task_queue_mutex_.lock();
	{
		// remove all tasks from the queue
		while( !tasks_.empty() )
			tasks_.pop();
	}
	task_queue_mutex_.unlock();

	// reenable access
	clear_queue_checkpoint_.open();
}

void Farm::create_workers(int num_workers)
{
	// close any threads already running
	join_workers();

	// create some threads to calculate the mandelbrot
	for( int t = 0; t < num_workers; t++ ) {
		create_worker();
	}
}

void Farm::create_worker()
{
	finished_work_ = false;

	// create a new therad
	threads_.push_back(new std::thread(std::mem_fun(&Farm::do_work), this));
}

bool Farm::is_done() const
{
	return tasks_.empty();
}

void Farm::join_workers()
{
	finished_work_ = true;

	for( auto& t : threads_ ) {
		t->join();
	}

	threads_.clear();
}

void Farm::do_work()
{
	// keep working untill we are told to stop
	while( !finished_work_ )
	{
		// check if the queue is safe to access
		clear_queue_checkpoint_.pass();
		
		// protect the queue, get the next task from it
		task_queue_mutex_.lock();

		// if there is anything in the task queue, run it
		if( !tasks_.empty() )
		{
			Task* t = tasks_.front();
			tasks_.pop();

			task_queue_mutex_.unlock();

			if( t )			// check t is valid
				t->run();	// execute the task

			delete t;
		}
		else
		{
			task_queue_mutex_.unlock();

			// sleep so as not to busy-wait
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}