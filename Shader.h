#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>

using namespace glm;

class Shader
{
private:
	static GLint activeProgram;
	GLint program;

public:
	Shader(
		const GLchar * vertexPath,
		const GLchar * fragmentPath);

	Shader(
		const GLchar * vertexPath,
		const GLchar * fragmentPath,
		const GLchar * geometryPath);

	~Shader();

private:
	void catchShaderError(GLuint shader, const GLchar * path);
	GLuint loadShader(GLenum type, const GLchar * fileName);

public:
	void use();

	GLint getUniform(const GLchar * name);

	void setUniform(const GLchar * name, GLfloat value);
	void setUniform(const GLchar * name, const vec2 & vec);
	void setUniform(const GLchar * name, const vec3 & vec);
	void setUniform(const GLchar * name, const mat4 & mat);

	void setUniform(const GLchar * name, GLuint value);
	void setUniform(const GLchar * name, const uvec2 & vec);
};