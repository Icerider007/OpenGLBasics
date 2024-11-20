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
	
	//Positions of the triangle vertices
	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 1.0f,//Lower left vertex
		0.5f,-0.5f * float(sqrt(3)) / 3, 0.0f,//Lower right vertex
		0.0f,0.5f * float(sqrt(3)) * 2/3, 0.0f,//Top vertex
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

	// This creates the vertex shader object and loads in the code
	// that was at the start of this file
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertex_shader);

	//Loads in the fragment shader object, and once again, puts in the code
	// that we saw at the beginning of the file, puts it in a fragment shader object
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragment_shader);

	//Creates a shader program to use the shaders
	GLuint shader_program = glCreateProgram();

	//Attaches the vertex shader to the shader program
	glAttachShader(shader_program, vertex_shader);
	//Attaches fragment shader to shader program
	glAttachShader(shader_program, fragment_shader);

	//Links program, so it compiles and runs with the rest of the code I guess?
	glLinkProgram(shader_program);

	//Now that we've loaded the shaders into the shader program, we can delete the shader objects
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	//We're initializing the VertexArrayObject and the VertexBufferObject
	//The array stores all the points in a matrix, and the buffer, well
	//that's what we use to load the points? Before we draw them I mean
	GLuint VAO,VBO;

	glGenVertexArrays(1, &VAO);

	//So, I don't know if this is necessary, but basically, binding the VAO 
	//makes it so it knows which array to draw from? To refer to and stuff?
	//Tried commenting it out and it didn't work
	glBindVertexArray(VAO);

	//This generates a buffer with only one object
	glGenBuffers(1, &VBO);
	//This makes it so it knows which buffer to refer to, same as the other bind function
	//Also specifies that it's an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//This puts the VAO data into the buffer (VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	//This uh, it configures the vertex attributes so OpenGL knows how to read
	//the buffer. Cause it needs to know 
	//glVertexAttribPointer(index_start,size_of_array,typeofarray,isthearraynormalized,stride?,uh_pointer_leading_to_array_index)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3*sizeof(float),(void*)0);
	//This enables the vertex attrib array and shows OpenGl which index to start reading from
	glEnableVertexAttribArray(0);

	//Um, this binds both the VAO and VBO to 0 so we don't accidentally modify them?
	//Binds VBO to 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Binds VAO to 0
	glBindVertexArray(0);

	//Eh, decides what the viewport is
	glViewport(0, 0, WIDTH, HEIGHT);

	//Keeps the glfwWindow open and it keeps polling events until we click the "X" button on the window
	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();
	
		//Rn, it just changes the background color. Idk what it's normally used for
		glClearColor(0.47f, 0.14f, 0.17f, 1.0f);
		//This is the thing that actually cleans the back buffer so the front buffer
		//can be drawn into it
		glClear(GL_COLOR_BUFFER_BIT);
		//Tells OpenGL to use that one shader program
		glUseProgram(shader_program);
		//Binds vertex array over and over again
		glBindVertexArray(VAO);
		//Does the thing to draw the array
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
