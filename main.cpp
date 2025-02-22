#include <math.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "camera.h"

const int WIDTH = 800;
const int HEIGHT= 800;


int main() {
	//Starts glfw, I guess? What does that even mean lol
	std::cout << "Initializing glfw..\n" ;
	glfwInit();

	//Tells glfw openGL version and type
	std::cout << "Setting glfw window hints\n";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//Creates the window, and at the same time, adds title and stuff
	std::cout << "Creating window\n";
	GLFWwindow* win = glfwCreateWindow(WIDTH, HEIGHT, "Winner",NULL,NULL);

	//Error check
	if (win == NULL) {
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return 1;
	}
	
	std::cout << "Window created successfully\n";
	//Makes it so the current context is this window, so all
	//commands apply to this one. I think?
	glfwMakeContextCurrent(win);

	//Positions of the triangle vertices
	GLfloat vertices[] = {
		//			Co-ordinates						
		-0.5,	-0.5,	0.5,	//0	
		 0.5,	-0.5,	0.5,	//1
		-0.5,	 0.5,	0.5,	//2
		 0.5,	 0.5,   0.5,	//3	
		-0.5,	-0.5,	-0.5,	//4
         0.5,	-0.5,	-0.5,	//5
        -0.5,	 0.5,	-0.5,	//6
         0.5,	 0.5,   -0.5,	//7
	};

	GLfloat color[] = {
		//Colors
		1.0f, 0.0f, 0.0f, //0 - Red
		0.0f, 1.0f, 0.00f, //1 - Green
		0.0f, 0.0f, 1.00f, //2 - Blue
		1.0f, 1.00f, 1.00f, //3 White
	};

	GLfloat texPos[] = {
		0.0f, 0.0f, //0
		1.0f, 0.0f, // 1
		0.0f, 1.0f, // 2
		1.0f, 1.0f, // 3

		0.0f, 0.0f, // 4
        1.0f, 0.0f, // 5
        0.0f, 1.0f, // 6
        1.0f, 1.0f, // 7
	};

	GLuint indices[] = {
		0, 1, 2,  // Front face
		1, 3, 2,
		4, 5, 6,  // Back face
		5, 7, 6,
		0, 4, 2,  // Left face
		4, 6, 2,
		1, 5, 3,  // Right face
		5, 7, 3,
		2, 3, 6,  // Top face
		3, 7, 6,
		0, 1, 4,  // Bottom face
		1, 5, 4
	};
	
	//"Loads glad", and glad has a shit ton of OpenGL functions 
	gladLoadGL();

	//Shaders
	std::cout << "Loading shaders..\n" ;
	Shader shaderProgram("../default.vert","../default.frag");

	
	//Textures
	int imgWidth, imgHeight, imgColorNum;

	stbi_set_flip_vertically_on_load(true);
	//Loads the texture into bytes. STBI_rgb_alpha means all color channels are supported
	unsigned char* bytes = stbi_load("../textures/test.png", &imgWidth, &imgHeight, &imgColorNum, STBI_rgb_alpha);
	//Initializes and generates texture
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Set filtering modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Specify texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	//Creates a texture image on the current GL_TEXTURE_2D
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	
	//Generate mip maps
	//glGenerateMipmap(GL_TEXTURE_2D);

	//stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(tex0Uni, 0);
	
	//Specifies the viewport within which OpenGL renders will be displayed maybe?
	glViewport(0, 0, WIDTH, HEIGHT);

	//We're initializing the VertexArrayObject and the VertexBufferObject
	//The array stores all the points in a matrix, and the buffer, well
	//that's what we use to load the points? Before we draw them I mean

	GLuint VAO,VBO_pos,VBO_color,VBO_texPos,EBO; //Initialize Vertex Array Object,
						//			 Vertex Buffer Object,
						//			 Element Buffer Object


	//This generates a buffer with only one object
	//Generates reference value
	std::cout << "Generating buffers..\n";
	glGenBuffers(1, &VBO_pos);
	glGenBuffers(1, &VBO_color);
	glGenBuffers(1, &VBO_texPos);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	
	//This makes it so it knows which buffer to refer to, same as the other bind function
	//Also specifies its buffer type
	std::cout << "Binding buffers..\n";
	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//This puts the VAO data into the buffer (VBO_pos)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//This uh, it configures the vertex attributes so OpenGL knows how to read
	//the buffer. Cause it needs to know 
	//glVertexAttribPointer(index_start,size_of_array,typeofarray,isthearraynormalized,stride?,uh_pointer_leading_to_array_index)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3*sizeof(float),(void*)0);
	//This enables the vertex attrib array and shows OpenGl which index to start reading from
	glEnableVertexAttribArray(0);

	//Real quick binding VBO_color to GL_ARRAY_BUFFER and then passing it onto 
	//layout 1 of VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	//Binding VBO_texPos to GL_ARRAY_BUFFER and then passing it into layout 2 of VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO_texPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texPos), texPos, GL_STATIC_DRAW);
	glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	//This unbinds the buffers and arrays
	//Binds VBO_pos to 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Binds VAO to 0
	glBindVertexArray(0);
	//Binds EBO to 0, must be done after unbinding VAO because
	//EBO is linked in VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Camera
	Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

	//Enables depth buffer
	glEnable(GL_DEPTH_TEST);
	
	//Keeps the glfwWindow open and it keeps polling events until we click the "X" button on the window
	std::cout << "Starting render loop\n";
	while (!glfwWindowShouldClose(win)) {

		glfwSwapInterval(1);	
		//Rn, it just changes the background color. Idk what it's normally used for
		glClearColor(0.47f, 0.14f, 0.17f, 1.0f);
		
		//This is the thing that actually cleans the back buffer so the front buffer
		//can be drawn into it. Including the DEPTH_BUFFER, I guess?
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Tells OpenGL to use that one shader program
		shaderProgram.Activate();

		//Checks for camera inputs
		camera.Inputs(win);
		//Updates and exports camera matrix to vertex shader
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		//Binds texture
		glBindTexture(GL_TEXTURE_2D, texture);
		
		//Binds vertex array over and over again
		glBindVertexArray(VAO);
		//Draws triangles using the VAO
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int) , GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	
	//Cleans up 
	std::cout << "Cleaning up arrays and buffers\n";
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO_pos);
	glDeleteBuffers(1, &VBO_color);
	glDeleteBuffers(1, &VBO_texPos);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &texture);
	shaderProgram.Delete();
	//Kills the window lol, cleans up resources I guess
	glfwDestroyWindow(win);
	//Ends glfw stuff, I guess?
	glfwTerminate();

	return 0;
}
