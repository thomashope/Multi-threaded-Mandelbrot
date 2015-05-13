#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <SDL.h>

enum Key {left, right, up, down, plus, minus, space,
	one, two, three, four, five, six, seven, eight,
	c, b, m, r, s, enter, num_1, num_2, num_3, num_4 };

extern std::vector<bool> key_pressed;

void init_keys();

void set_keys(const SDL_Event& e);

void clear_keys();

#endif