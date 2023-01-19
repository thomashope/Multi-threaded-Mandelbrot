#include <iostream>
#include <SDL.h>
#include <thread>
#include "mandelbrot_line.h"
#include "mandelbrot_task.h"
#include "mix_lines_task.h"
#include "shuffle_lines_task.h"
#include "bubble_sort_task.h"
#include "check_sorted_task.h"
#include "start_timer_task.h"
#include "stop_timer_task.h"
#include "farm.h"
#include "input.h"
#include "viewport.h"

using std::cout;
using std::endl;
using std::chrono::steady_clock;

// screen setup
const int WIDTH{ 800 };
const int HEIGHT{ 600 };
SDL_Event event;			// for processing window messages
SDL_Window* win;			// main application window
SDL_Renderer* ren;			// render target
SDL_Texture* screen;		// texture to write buffer data too
std::mutex screen_mutex;	// prevents multiple threads from accessing the screen 

std::mutex epoch_mutex;		// prevents multiple threads from using the same time since epoch
std::chrono::steady_clock::time_point timer;

const int MAX_ITERATIONS{ 500 };	
MandelbrotData MbrotData;			// describes what area of the mandelbrot should be drawn
int dispersion{ 1 };				// controlls how far apart lines are when drawn

std::vector<MandelbrotLine*> Mandelbrot;

// distributes tasks across multiple worker threads
Farm farm;

void create_window();

// sends tasks to worker threads to redraw the mandelbrot
void launch_mandelbrot_tasks();

void handle_input();

void reinitalise_threads(int num_threads);

// swaps the given number of lines 'randomly'
void mix_image(int swaps);

// preforms std::shuffle on all lines of the mandelbrot
void shuffle_image();

// use bubble sort to get the mandelbrot back to how it should be
void sort_image();

// resets to all black
void redraw_image();

int main(int argc, char* argv[])
{
	// set up all the lines of the mandelbrot
	for( int h = 0; h < HEIGHT; h++ ) {
		Mandelbrot.push_back( new MandelbrotLine(h) );
	}

	launch_mandelbrot_tasks();

	farm.create_workers( std::thread::hardware_concurrency() - 1 );

	// a window to display the result in
	create_window();

	init_keys();

	// clear screen to black
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);

	bool quit{ false };

	while( !quit ) {	// START OF LOOP
		while( SDL_PollEvent(&event) ) { // poll the event queue for quit events
			if( event.type == SDL_QUIT ) {
				quit = true;
			}
			else if( event.type == SDL_KEYDOWN ) {
				if( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE ) { quit = true; }
				set_keys(event);
			}
		}

		handle_input();

		// draw each line to the screen texture
		for( size_t i = 0; i < Mandelbrot.size(); i++ ) {
			Mandelbrot[i]->draw(screen, &screen_mutex, i);
		}

		// copy the screen texture to the window
		screen_mutex.lock();
		{
			SDL_RenderCopy(ren, screen, NULL, NULL);
			SDL_RenderPresent(ren);
		}
		screen_mutex.unlock();

		clear_keys();
	}	// END OF LOOP

	// join the worker threads
	farm.join_workers();

	return 0;
}

void handle_input()
{
	// navigate the mandelbrot
	if( key_pressed[left]	)	{ move_viewport_left();		launch_mandelbrot_tasks(); }	
	if( key_pressed[right]	)	{ move_viewport_right();	launch_mandelbrot_tasks(); }
	if( key_pressed[up]		)	{ move_viewport_up();		launch_mandelbrot_tasks(); }
	if( key_pressed[down]	)	{ move_viewport_down();		launch_mandelbrot_tasks(); }
	if( key_pressed[plus]	)	{ zoom_viewport_in();		launch_mandelbrot_tasks(); }
	if( key_pressed[minus]	)	{ zoom_viewport_out();		launch_mandelbrot_tasks(); }
	if( key_pressed[space]	)	{ reset_viewport();			launch_mandelbrot_tasks(); }

	// redraw, scramble and sort the image
	if( key_pressed[r] ) { redraw_image();		cout << "Redraw" << endl; }
	if( key_pressed[s] ) { shuffle_image();		cout << "Shuffle image" << endl; }
	if( key_pressed[m] ) { mix_image(100);		cout << "Mix image" << endl; }
	if( key_pressed[b] ) { sort_image();		cout << "Bubble sort" << endl; }
	if( key_pressed[c] ) { farm.clear_tasks();	cout << "Clear tasks" << endl; }

	// use number keys to change how many threads are running
	if( key_pressed[one]	) { reinitalise_threads(1); }
	if( key_pressed[two]	) { reinitalise_threads(2); }
	if( key_pressed[three]	) { reinitalise_threads(3); }
	if( key_pressed[four]	) { reinitalise_threads(4); }
	if( key_pressed[five]	) { reinitalise_threads(5); }
	if( key_pressed[six]	) { reinitalise_threads(6); }
	if( key_pressed[seven]	) { reinitalise_threads(7); }
	if( key_pressed[eight]	) { reinitalise_threads(8); }

	// change line dispersion with the num_pad
	if( key_pressed[num_1] ) { dispersion = 1; cout << "Dispersion = " << dispersion << endl; }
	if( key_pressed[num_2] ) { dispersion = 2; cout << "Dispersion = " << dispersion << endl; }
	if( key_pressed[num_3] ) { dispersion = 3; cout << "Dispersion = " << dispersion << endl; }
	if( key_pressed[num_4] ) { dispersion = 4; cout << "Dispersion = " << dispersion << endl; }
}

void reinitalise_threads(int num_threads)
{
	farm.join_workers();
	farm.create_workers(num_threads);
	cout << num_threads << " threads running" << endl;
}

void mix_image(int swaps)
{
	while( swaps-- > 0 ) {
		farm.add_task(new MixLinesTask(&Mandelbrot, &epoch_mutex));
	}
}

void shuffle_image()
{
	farm.add_task(new ShuffleLinesTask(&Mandelbrot));
}

void sort_image()
{
	int bubbles = HEIGHT;

	// start the timer
	farm.add_task(new StartTimerTask(&timer));

	// add HEIGH sort tasks to the task queue
	while( bubbles-- > 0 ) {
		farm.add_task(new BubbleSortTask(&Mandelbrot));
	}

	// HEIGHT iterations should be enough, but check just in case
	farm.add_task(new CheckSortedTask(&Mandelbrot, &farm, &timer));

	// check sorted task will stop the timer when sorting is done
}

void redraw_image()
{
	for( auto l : Mandelbrot ) {
		for( int i = 0; i < WIDTH; i++ ) {
			l->data_[i] = 0;
		}
	}

	farm.clear_tasks();

	// add a start and end time task around the mandelbrot
	// to calculate the time taken to render (roughly)
	farm.add_task(new StartTimerTask(&timer));

	// dispersion controlls much to space apart lines are when drawn
	for( int d = 0; d < dispersion; d++ ) {
		// pass each line to a task then the task into the farm
		for( size_t l = 0; l < Mandelbrot.size(); l++ ) {
			// spread the lines out
			if( (l % dispersion) - d == 0 ) {
				farm.add_task(new MandelbrotTask(Mandelbrot[l]));
			}
		}
	}

	farm.add_task(new StopTimerTask(&timer));
}

void launch_mandelbrot_tasks()
{
	// ensue the farm is empty to begin with
	farm.clear_tasks();
	
	// dispersion controlls much to space apart lines are when drawn
	for( int d = 0; d < dispersion; d++ ) {
		// pass each line to a task then the task into the farm
		for( size_t l = 0; l < Mandelbrot.size(); l++ ) {
			// spread the lines out
			if( (l % dispersion) - d == 0 ) {
				farm.add_task(new MandelbrotTask(Mandelbrot[l]));
			}
		}
	}
}

void create_window()
{
	// set up sdl
	if( SDL_Init(SDL_INIT_EVERYTHING) ) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		SDL_Delay(1000);
		exit(-1);
	}

	// create the window
	win = SDL_CreateWindow(
		"Thoams Hope: 1302495",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIDTH,
		HEIGHT,
		SDL_WINDOW_SHOWN );

	// check the window for errors
	if( !win ) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Delay(1000);
		exit(-1);
	}

	// create a render target
	ren = SDL_CreateRenderer( win, -1, SDL_RENDERER_ACCELERATED);

	// check the render target for errors
	if( !ren ) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Delay(1000);
		exit(-1);
	}

	// create the texture to copy data into
	screen = SDL_CreateTexture(
		ren,
		SDL_GetWindowPixelFormat(win),
		SDL_TEXTUREACCESS_STREAMING,
		WIDTH,
		HEIGHT );
}