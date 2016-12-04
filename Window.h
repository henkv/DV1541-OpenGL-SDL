#pragma once
#include <SDL.h>
#include <glm\glm.hpp>

using namespace glm;

class Window
{
private:
	SDL_Window * window;

public:
	Window(const char * name, int width, int height);
	~Window();

private:
	void SetGLAttributes();
	SDL_Window * createWindow(const char * name, int width, int height);

public:
	void handleEvents();
	void draw();
	void clear();

	vec2 getResolution();
	vec2 getMouseCoords();
};

