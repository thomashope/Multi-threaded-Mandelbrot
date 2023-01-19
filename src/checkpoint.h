#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include <thread>
#include <chrono>
#include <atomic>

// checkpoints are a type of syncronisation object
// similar to a barrier but they are designed for situations
// where threads should only blocks occasionaly so some special task can complete

class Checkpoint
{
public:
	Checkpoint() { lock_ = false; }
	
	// blocks if the checkpoint is closed
	inline void pass() const { while( lock_ ) std::this_thread::sleep_for(std::chrono::milliseconds(5)); }

	// causes threads to block next time they call pass()
	inline void close() { lock_ = true; }

	// releases all threads blocking on this checkpoint
	inline void open() { lock_ = false; }

private:
	std::atomic_bool lock_;
};

#endif