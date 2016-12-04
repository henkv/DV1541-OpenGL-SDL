#include <stdexcept>
#include <Windows.h>
#include <sstream>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Shader.h"
#include "Window.h"
#include "Texture.h"

#include "bth_image.h"

using namespace glm;

void createTriangData();

GLuint gVertexBuffer = 0;
GLuint gVertexAttribute = 0;

const mat4 I;
const vec3 X(1, 0, 0);
const vec3 Y(0, 1, 0);
const vec3 Z(0, 0, 1);
const vec3 O(0, 0, 0);


int main(int argc, char ** argv)
{
	try
	{
		// Create the window
		Window window = { "DV1541 OpenGL SDL", 800, 600 };

		// Initialize Glew
		glewExperimental = GL_TRUE;
		glewInit();

		glClearColor(0, 0, 0, 1); 
		glEnable(GL_DEPTH_TEST);

		createTriangData();

		Shader shader = { 
			"VertexShader.glsl", 
			"FragmentShader.glsl",
			"GeometryShader.glsl"
		};
		shader.use();

		Texture texture = {
			(GLsizei)BTH_IMAGE_WIDTH, 
			(GLsizei)BTH_IMAGE_HEIGHT, 
			(GLubyte*)BTH_IMAGE_DATA
		};
		texture.use();

		shader.setUniform_mat4("view", lookAt(vec3(0, 0, -2), O, Y));
		shader.setUniform_mat4("proj", perspective(pi<float>() * 0.45f, 8.f / 6.f, 0.1f, 20.f));
		shader.setUniform_vec3("offset", vec3(0, 0, 0.33));

		shader.setUniform_vec3("lightPosition", vec3(0, 0, -10));
		shader.setUniform_vec3("lightColor",	vec3(1));
		shader.setUniform_vec3("ambientLight",  vec3(0.0f));

		vec2 mousePos;
		mat4 world;
		while (true)
		{
			window.handleEvents();
			window.clear();

			mousePos = window.getMouseCoords();


			world = rotate<GLfloat>(I, half_pi<float>() * mousePos.x, Y);
			world = rotate<GLfloat>(world, 0.5f * half_pi<float>() * (-mousePos.y), X);
			shader.setUniform_mat4("world", world);



			glBindVertexArray(gVertexAttribute);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			window.draw();
		}
	}
	catch (const std::runtime_error &error)
	{
		std::wostringstream msg;
		msg << error.what() << std::endl;
		OutputDebugStringW(msg.str().c_str());
	}

	return 0;
}

void createTriangData()
{
	struct Vertex
	{
		vec3 postion;
		vec3 color;
		vec2 uv;
	};

	Vertex vertices[] = {
		{{ -1,  1, 0 }, { 1, 0, 0 }, { 1, 0 }},
		{{  1,  1, 0 }, { 0, 1, 0 }, { 0, 0 }},
		{{ -1, -1, 0 }, { 0, 0, 1 }, { 1, 1 }},
		{{  1, -1, 0 }, { 1, 1, 1 }, { 0, 1 }}
	};	

	// Create an internal Vertex Array Object (VAO) and bind it for manipulation
	glGenVertexArrays(1, &gVertexAttribute);
	glBindVertexArray(gVertexAttribute);

	// Create an internal Vertex Buffer Object (VBO) and bind it for manipulation
	// "maybe" copy the vertices to the gpu
	glGenBuffers(1, &gVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// write vertex data to memory
	// location 0 position
	// location 1 color
	// location 2 texture coordinate
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(vec3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(vec3) * 2));

}


