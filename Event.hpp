#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#pragma once

//andrzejek 


class Event
{
	int mouseDown;
	int mouseUp;
	char keypressed;
	SDL_Event event;
public:
	Event();
	glm::vec2 mousePos;
	char keypress;
	char oneclick;
	int mouseClick();
	bool quitbutt();
	bool poll();
	~Event();
};

