#include "Window.h"
#include <stdexcept>
#include <GL\glew.h>

Window::Window(const char * name, int width, int height)
{
	window = createWindow(name, width, height);
}
Window::~Window()
{
}

void Window::SetGLAttributes()
{
	// Set the active OpenGL version to 3.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Make sure we can only use the newest version of the api
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Set the render mode to doubble buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Enable vsync
	SDL_GL_SetSwapInterval(1);
}
SDL_Window * Window::createWindow(const char * name, int width, int height)
{
	SDL_Window *window;

	// Try to initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::runtime_error("Unable to initialize SDL");

	// Create an SDL Window
	window = SDL_CreateWindow(
		name,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL
	);

	// Check if creation was successful
	if (!window)
		throw std::runtime_error("Failed to create Window");

	// Set OpenGL Settings
	SetGLAttributes();

	// Try to create a GL Context in window
	if (!SDL_GL_CreateContext(window))
		throw std::runtime_error("Failed to create GL Context");

	return window;
}

void Window::handleEvents()
{
	SDL_Event windowEvent = { 0 };

	// Poll all events sent to window
	while (SDL_PollEvent(&windowEvent)) 
	{
		switch (windowEvent.type)
		{
		case SDL_QUIT:
			SDL_Quit();
			exit(0);
			break;
		}
	}
}
void Window::draw()
{
	SDL_GL_SwapWindow(window);
}
void Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

vec2 Window::getResolution()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	return vec2(w, h);
}

vec2 Window::getMouseCoords()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	vec2 resolution = getResolution();
	vec2 mousePos(x, y);

	mousePos.x /= resolution.x;
	mousePos.y /= resolution.y;
	mousePos *= 2.f;
	mousePos -= 1.f;

	return mousePos;
}

