#include <glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "marl.h"

#define PI 3.14159265358979323846

int main(int argc, char **argv) {
	
	
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	MARL_Loop *loop = MARL_CreateLoop(16);
	MARL_Window *window = MARL_CreateWindow("Hello World", 0, 0, 1600, 900);
	SDL_GLContext context = SDL_GL_CreateContext(window->sdlWindow);
	SDL_GL_SetSwapInterval(1);
	int version = gladLoadGL();
//	printf("GL %d.%d\n", GLAD_GL_VERSION_MAJOR(version), GLAD_GL_VERSION_MINOR(version));
	
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,	// bl
		0.5f, -0.5f, 0.0f,	// br
		-0.5f, 0.5f, 0.0f,	// tl
		0.5f, 0.5f, 0.0f,	// tr
	};

	unsigned int indices[] = {
		0,1,2,
		1,2,3,
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	
	unsigned int VBO;
	glGenBuffers(1, &VBO);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success) {
		    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s\n", infoLog);
	}


	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s\n", infoLog);
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		    printf("Failed to create shaderprogram:\n%s\n", infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);  

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	short isRunning = 1;
	SDL_Event event;
	long second = SDL_GetTicks64() + 1000;
	long ticks = 0;
	int loopAction = 0;

	int mouseX = 0;
	int mouseY = 0;
	int evtype = 0;
	while(isRunning) {
		loopAction = MARL_LoopWait(loop, &event);
		if(loopAction == 1) {
			if(event.type != evtype) {
				SDL_Log("-> %i\n", event.type);
				SDL_Log("---> %X\n", event.tfinger);
				SDL_Log("[[%b]]", event);
				evtype = event.type;
			}
			if(event.type == SDL_QUIT) {
				isRunning = 0;
			} else if(event.type == SDL_KEYDOWN) {
				SDL_Log("[%5i] Found Key Event!!!", loop->iteration);
			} else if(event.type == SDL_KEYUP) {
				SDL_Log("[%5i] Key went up!!!", loop->iteration);
			} else if(event.type == SDL_MOUSEMOTION) {
				mouseX = event.motion.x;
				mouseY = event.motion.y;
			} else if(event.type == SDL_FINGERDOWN) {
				SDL_Log("%i, %i, %f\n", event.tfinger.touchId, event.tfinger.fingerId, event.tfinger.pressure);
			}
		} else {
		//	glClearColor((0.5+0.5f*SDL_sinf(2* PI * loop->iteration/60)), 0.1f, 0.5+0.5*SDL_cosf(2*PI* loop->iteration/60), 1.0f);
			int renderHeight;
			SDL_GetRendererOutputSize(window->sdlRenderer, NULL, &renderHeight);
			glViewport(mouseX, renderHeight - mouseY - 100,100,100);		
			glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
		//	glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			SDL_GL_SwapWindow(window->sdlWindow);
		}
	} 

	MARL_DestroyLoop(loop);
	MARL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
