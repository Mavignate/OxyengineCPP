#include "Window.hpp"
#include <iostream>

Window::Window(std::string title, int width, int height, bool fullscreen = 0)
:	w(width),
	h(height)
{
	bool success = true;
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Use OpenGL 3.1 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create context
			context = SDL_GL_CreateContext(window);
			if (context == NULL)
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize GLEW
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK)
				{
					printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
				}

				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

	
			}
		}
	}
	int framex = w;
	int	framey = h;

	glViewport(0, 0, framex, framey);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int Window::getHeight()
{
	return h;
}

int Window::getWidth()
{
	return w;
}

bool Window::isOpen()
{
	return open;
}

void Window::vsync(bool boolvsync)
{
	if (SDL_GL_SetSwapInterval(boolvsync) < 0) {
		std::cerr << "Error Set vsync: " << SDL_GetError();
	}
}

void Window::quit()
{
	open = false;
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Window::swap()
{
	SDL_GL_SwapWindow(window);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::setColor(glm::vec4 color)
{
	glClearColor((float)color.r, (float)color.g, (float)color.b, (float)color.a);
}

Window::~Window()
{
}
