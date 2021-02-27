#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <string>
#include <glm/glm.hpp>

#pragma once
class Window
{
	SDL_Window* window;
	SDL_GLContext context;
	bool open = true;
	int w, h;
public:
	Window(std::string title, int width, int height, bool fullscreen);
	int getHeight();
	int getWidth();
	bool isOpen();
	void vsync(bool boolvsync);
	void quit();
	void swap();
	void setColor(glm::vec4 color);
	~Window();
};

