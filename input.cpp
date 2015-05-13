#include "input.h"
#include <algorithm>

// holds all keystates
std::vector<bool> key_pressed;

void init_keys()
{
	// make sure the array has enough space
	while( key_pressed.size() < 64 ) {
		key_pressed.push_back(false);
	}
}

void set_keys(const SDL_Event& e)
{
	// set elements to true if pressed
	switch( e.key.keysym.scancode )
	{
	case SDL_SCANCODE_LEFT: key_pressed[left] = true; break;
	case SDL_SCANCODE_RIGHT: key_pressed[right] = true; break;
	case SDL_SCANCODE_UP: key_pressed[up] = true; break;
	case SDL_SCANCODE_DOWN: key_pressed[down] = true; break;
	case SDL_SCANCODE_EQUALS: key_pressed[plus] = true; break;
	case SDL_SCANCODE_MINUS: key_pressed[minus] = true; break;
	case SDL_SCANCODE_SPACE: key_pressed[space] = true; break;
	case SDL_SCANCODE_1: key_pressed[one] = true; break;
	case SDL_SCANCODE_2: key_pressed[two] = true; break;
	case SDL_SCANCODE_3: key_pressed[three] = true; break;
	case SDL_SCANCODE_4: key_pressed[four] = true; break;
	case SDL_SCANCODE_5: key_pressed[five] = true; break;
	case SDL_SCANCODE_6: key_pressed[six] = true; break;
	case SDL_SCANCODE_7: key_pressed[seven] = true; break;
	case SDL_SCANCODE_8: key_pressed[eight] = true; break;
	case SDL_SCANCODE_KP_1: key_pressed[num_1] = true; break;
	case SDL_SCANCODE_KP_2: key_pressed[num_2] = true; break;
	case SDL_SCANCODE_KP_3: key_pressed[num_3] = true; break;
	case SDL_SCANCODE_KP_4: key_pressed[num_4] = true; break;
	case SDL_SCANCODE_RETURN: key_pressed[enter] = true; break;
	case SDL_SCANCODE_C: key_pressed[c] = true; break;
	case SDL_SCANCODE_R: key_pressed[r] = true; break;
	case SDL_SCANCODE_S: key_pressed[s] = true; break;
	case SDL_SCANCODE_B: key_pressed[b] = true; break;
	case SDL_SCANCODE_M: key_pressed[m] = true; break;
	default: break;
	}
}

void clear_keys()
{
	// set all keys to false
	std::fill(key_pressed.begin(), key_pressed.end(), false);
}