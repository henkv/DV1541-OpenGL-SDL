#include <SDL.h>
#include <GL/glew.h>
#include <fstream>

/*
Hello! I've been working on Assignment 2 at home with OpenGL and decided to use the newest version of SDL and Glew. I get an access violation error while trying to call: glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

Exception thrown at 0x0000000053912D79 (nvoglv64.dll) in DV1541-OpenGL-SDL.exe: 0xC0000005: Access violation reading location 0x0000000000000000.

While googeling for an answer I found out that its mostly due to diver errors according to this page:
https://www.vegascreativesoftware.info/us/forum/vegas12-keeps-crashing-with-nvoglv64-dll-exception--97917/

I've tried using my code in the project provided and it works there.
*/

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

void GL_Die(const char *errorMessage);
void SDL_Die(const char *errorMessage);
void SDL_GL_SetAttributes();
SDL_Window* createWindow(const char *name, int width, int height);
void handleWindowEvents(SDL_Window *window);

GLuint loadShader(GLenum type, const char *fileName);
void catchShaderError(GLuint shader);

void createShaderProgram();
void createTriangData();


SDL_Window *mainWindow;

GLuint gVertexBuffer = 0;
GLuint gVertexAttribute = 0;
GLuint gShaderProgram = 0;


int main(int argc, char ** argv)
{
	// Create the window
	mainWindow = createWindow("DV1541 OpenGL SDL", 640, 480);

	// Initialize Glew
	glewInit();
	//glViewport(0, 0, 640, 480);
	glClearColor(0, 0, 0, 1);

	createShaderProgram();
	createTriangData();

	while (true)
	{
		// Take care of window events
		handleWindowEvents(mainWindow);

		// Clear the back buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the shaders
		glUseProgram(gShaderProgram);
		glBindVertexArray(gVertexAttribute);


		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


		// Swap the render buffers to show changes in the window.
		SDL_GL_SwapWindow(mainWindow);
	}

	return 0;
}


void GL_Die(const char *errorMessage)
{
	// Create a message box to show the error
	SDL_ShowSimpleMessageBox(
		SDL_MESSAGEBOX_ERROR,
		"GL Died",
		errorMessage,
		NULL);

	// Exit application
	SDL_Quit();
	exit(-1);
}

void SDL_Die(const char *errorMessage)
{
	// Create a message box to show the error
	SDL_ShowSimpleMessageBox(
		SDL_MESSAGEBOX_ERROR,
		errorMessage,
		SDL_GetError(),
		NULL);

	// Exit application
	SDL_Quit();
	exit(-1);
}

void SDL_GL_SetAttributes()
{
	// Set the active OpenGL version to 3.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Make sure we can only use the newest version of the api
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Set the render mode to doubble buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

SDL_Window* createWindow(const char *name, int width, int height)
{
	SDL_Window *window;

	// Try to initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		SDL_Die("Unable to initialize SDL");

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
		SDL_Die("Failed to create Window");

	// Set OpenGL Settings
	SDL_GL_SetAttributes();
	SDL_GL_SetSwapInterval(1);

	// Try to create a GL Context in window
	if (!SDL_GL_CreateContext(window))
		SDL_Die("Failed to create GL Context");

	return window;
}

GLuint loadShader(GLenum type, const char * fileName)
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
	catchShaderError(shader);

	return GLuint();
}

void catchShaderError(GLuint shader)
{
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		char *log = new char[logSize];
		glGetShaderInfoLog(shader, logSize, &logSize, &log[0]);

		GL_Die(log);
	}
}

void createShaderProgram()
{
	// Create vertex and fragment shader
	GLuint vs = loadShader(GL_VERTEX_SHADER, "VertexShader.glsl");
	GLuint fs = loadShader(GL_FRAGMENT_SHADER, "FragmentShader.glsl");

	// Link shader program (connect vs and ps)
	gShaderProgram = glCreateProgram();
	glAttachShader(gShaderProgram, vs);
	glAttachShader(gShaderProgram, fs);
	glLinkProgram(gShaderProgram);
}

void createTriangData()
{
	struct Vertex
	{
		float x, y, z;
		float r, g, b;
	};

	Vertex vertices[4] = {
	//  {     x,     y,    z,    r,    g,    b }
		{ -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f },
		{  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f },
		{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f }
	};

	// Create an internal Vertex Array Object (VAO) and bind it for manipulation
	glGenVertexArrays(1, &gVertexAttribute);
	glBindVertexArray(gVertexAttribute);

	// Registers the first and second attribute for the VAO
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Create an internal Vertex Buffer Object (VBO) and bind it for manipulation
	glGenBuffers(1, &gVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	// "maybe" copy the vertices to the gpu
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// get index of vertex variables in shader 
	GLuint pData = glGetAttribLocation(gShaderProgram, "vertex_position");
	GLuint cData = glGetAttribLocation(gShaderProgram, "vertex_color");

	// write vertex data to memory
	glVertexAttribPointer(pData, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
	glVertexAttribPointer(cData, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 3));
}

void handleWindowEvents(SDL_Window *window)
{
	static SDL_Event windowEvent;

	// Poll all events sent to window
	while (SDL_PollEvent(&windowEvent)) {
		// Check what event type and handle appropriately
		switch (windowEvent.type)
		{
		case SDL_QUIT:
			SDL_Quit();
			exit(0);
			break;

		}
	}
}

