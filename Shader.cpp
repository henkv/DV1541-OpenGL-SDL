#include "Shader.h"
#include <fstream>
#include <stdexcept>
#include <glm\gtc\type_ptr.hpp>

GLint Shader::activeProgram = 0;

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	// Create vertex and fragment shader
	GLuint vs = loadShader(GL_VERTEX_SHADER, vertexPath);
	GLuint fs = loadShader(GL_FRAGMENT_SHADER, fragmentPath);

	// Link shader program (connect vs and ps)
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
}

Shader::Shader(
	const GLchar * vertexPath, 
	const GLchar * fragmentPath,
	const GLchar * geometryPath)
	: Shader(vertexPath, fragmentPath)
{
	// Create geometry shader
	GLuint gs = loadShader(GL_GEOMETRY_SHADER, geometryPath);

	// attach and link
	glAttachShader(program, gs);
	glLinkProgram(program);
}
Shader::~Shader()
{
}

void Shader::catchShaderError(GLuint shader, const GLchar * path)
{
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		char *log = new char[logSize];
		glGetShaderInfoLog(shader, logSize, &logSize, &log[0]);

		throw std::runtime_error(
			"shader compile error: " + 
			std::string(path) + "\n" + 
			std::string(log));
	}
}
GLuint Shader::loadShader(GLenum type, const GLchar * fileName)
{
	// Register a new shader
	GLuint shader = glCreateShader(type);

	// Open shader file and save its content in a string
	std::ifstream file(fileName);
	std::string fileContent(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	file.close();

	// Make a pointer to a cstring
	const char* fileContentPtr = fileContent.c_str();

	// Load shader code into graphics card and compile it
	glShaderSource(shader, 1, &fileContentPtr, nullptr);
	glCompileShader(shader);

	// Check if any erros occured and handle them
	catchShaderError(shader, fileName);

	return shader;
}

void Shader::use()
{
	glUseProgram(program);
	activeProgram = program;
}

GLint Shader::getUniform(const GLchar * name)
{
	if (program != activeProgram)
		throw std::runtime_error("not the active shader program");

	return glGetUniformLocation(program, name);
}

void Shader::setUniform(const GLchar * name, GLfloat value)
{
	glUniform1f(getUniform(name), value);
}
void Shader::setUniform(const GLchar * name, const vec2 & vec)
{
	glUniform2fv(getUniform(name), 1, value_ptr(vec));
}
void Shader::setUniform(const GLchar * name, const vec3 & vec)
{
	glUniform3fv(getUniform(name), 1, value_ptr(vec));
}
void Shader::setUniform(const GLchar * name, const mat4 & mat)
{
	glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, value_ptr(mat));
}

void Shader::setUniform(const GLchar * name, GLuint value)
{
	glUniform1ui(getUniform(name), value);
}
void Shader::setUniform(const GLchar * name, const uvec2 & vec)
{
	glUniform2uiv(getUniform(name), 1, value_ptr(vec));
}
