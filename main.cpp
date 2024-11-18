#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const int WIDTH = 1024;
const int HEIGHT= 768;


// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";


int main() {
	//Starts glfw, I guess? What does that even mean lol
	glfwInit();

	//Tells glfw openGL version. Shouldn't this come AFTER we specify profile?
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Tells glfw what type of OpenGL we're using, and that actually is important
	//because different versions of OpenGl have different functions. This one is 
	//supposed to have modern functions I guess
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLfloat vertices[] = {
		-0.7f, -0.7f * float(sqrt(3)) / 3, 0.0f,
		0.5f,0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f,0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
	};

	//Creates the window, and at the same time, adds title and stuff
	GLFWwindow* win = glfwCreateWindow(WIDTH, HEIGHT, "Winner",NULL,NULL);

	//Error check
	if (win == NULL) {
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return 1;
	}
	//This uhhh
	//"Introduces the current window to the context"? Wtf does that
	//even mean?
	glfwMakeContextCurrent(win);

	//"Loads glad", and glad has a shit ton of functions I guess 
	gladLoadGL();

	//Specifies the viewport within which OpenGL renders will be displayed maybe?
	glViewport(0, 0, WIDTH, HEIGHT);

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertex_shader);

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragment_shader);

	GLuint shader_program = glCreateProgram();

	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	GLuint VAO,VBO;

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glViewport(0, 0, WIDTH, HEIGHT);

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();
	
		//Rn, it just changes the background color. Idk what it's normally used for
		glClearColor(0.47f, 0.14f, 0.17f, 1.0f);
		//This is the thing that actually cleans the back buffer so the front buffer
		//can be drawn into it
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(win);

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader_program);

	//Kills the window lol, cleans up resources I guess
	glfwDestroyWindow(win);
	//Ends glfw stuff, I guess?
	glfwTerminate();

	return 0;
}
