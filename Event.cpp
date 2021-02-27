#include "Event.hpp"
#include <iostream>
#include <SDL2/SDL.h>


Event::Event()
{
   SDL_StopTextInput();
}

int Event::mouseClick()
{
    int ret = mouseUp;
    mouseUp = NULL;
    return ret;
}



bool Event::quitbutt(){
    return event.type == SDL_QUIT;
}


bool Event::poll(){
    if (SDL_PollEvent(&event)==1) {
        keypressed = keypress;
        mouseDown = NULL;
        oneclick = NULL;
        keypress = NULL;

        switch (event.type)
        {
        case SDL_KEYUP:
            if ((char)event.key.keysym.sym == keypressed)
            {
                oneclick = keypressed;
            }
                break;
        case SDL_KEYDOWN:
            keypress = (char)event.key.keysym.sym;
                break;
        case SDL_MOUSEMOTION:
            if (mousePos.x != event.motion.x && mousePos.y != event.motion.y)
            {
                mousePos = glm::vec2(event.motion.x, event.motion.y);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                mouseDown = 1;
                break;
            case SDL_BUTTON_MIDDLE:
                mouseDown = 2;
                break;
            case SDL_BUTTON_RIGHT:
                mouseDown = 3;
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                mouseUp = 1;
                break;
            case SDL_BUTTON_MIDDLE:
                mouseUp = 2;
                break;
            case SDL_BUTTON_RIGHT:
                mouseUp = 3;
                break;
            }
            break;
        }
        return true;
    }
    return false;
}

Event::~Event()
{
}
