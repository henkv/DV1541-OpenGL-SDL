#pragma once
#include <GL\glew.h>

class Texture
{
private:
	GLuint texture;

public:
	Texture(GLsizei width, GLsizei height, GLubyte * data);
	~Texture();

	void use();
};

