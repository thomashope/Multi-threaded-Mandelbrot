#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "mandelbrot_line.h"

extern MandelbrotData MbrotData;

// manipulates the visible area of the mandelbrot
void move_viewport_left();

void move_viewport_right();

void move_viewport_up();

void move_viewport_down();

void zoom_viewport_in();

void zoom_viewport_out();

void reset_viewport();

#endif