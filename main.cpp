#include <math.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "shaderClass.h"

const int WIDTH = 800;
const int HEIGHT= 800;


int main() {
	//Starts glfw, I guess? What does that even mean lol
	std::cout << "Initializing glfw..\n" ;
	glfwInit();

	//Tells glfw openGL version. Shouldn't this come AFTER we specify profile?
	std::cout << "Setting glfw window hints\n";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Tells glfw what type of OpenGL we're using, and that actually is important because different versions of OpenGl have different functions. This one is 
	//supposed to have modern functions I guess
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//Positions of the triangle vertices
	GLfloat vertices[] = {
		//			Co-ordinates						
		-1.0f/(float)sqrt(2),	-1.0f/(float)sqrt(2) ,	0.0f,			//0
		1.0f/(float)sqrt(2),	-1.0f/(float)sqrt(2) ,  0.0f,			//1
		-1.0f/(float)sqrt(2),	 1.0f/(float)sqrt(2)	,	0.0f,			//2
		1.0f/(float)sqrt(2),	 1.0f/(float)sqrt(2)	,	0.0f,		    //3
	};

	GLfloat color[] = {
		//Colors
		1.0f, 0.0f, 0.0f, //0 - Red
		0.0f, 1.0f, 0.00f, //1 - Green
		0.0f, 0.0f, 1.00f, //2 - Blue
		1.0f, 1.00f, 1.00f, //3 White
	};

	GLfloat texPos[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};

	GLuint indices[] = {
		0,1,2,	//Lower left triangle
		1,3,2,	//Lower right triangle
	};

	//Textures
	int imgWidth, imgHeight, imgColorNum;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load("../textures/image.png", &imgWidth, &imgHeight, &imgColorNum);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GLTEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParametri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParametri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParametri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParametri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(tex0Uni, 0);

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
	//This uhhh
	//"Introduces the current window to the context"? Wtf does that
	//even mean?
	glfwMakeContextCurrent(win);

	//"Loads glad", and glad has a shit ton of functions I guess 
	gladLoadGL();

	//Specifies the viewport within which OpenGL renders will be displayed maybe?
	glViewport(0, 0, WIDTH, HEIGHT);

	//Shaders
	std::cout << "Loading shaders..\n" ;
	Shader shaderProgram("../default.vert","../default.frag");
	
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


	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");


	//Keeps the glfwWindow open and it keeps polling events until we click the "X" button on the window
	std::cout << "Starting render loop\n";
	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();
	
		//Rn, it just changes the background color. Idk what it's normally used for
		glClearColor(0.47f, 0.14f, 0.17f, 1.0f);
		//This is the thing that actually cleans the back buffer so the front buffer
		//can be drawn into it
		glClear(GL_COLOR_BUFFER_BIT);
		//Tells OpenGL to use that one shader program
		shaderProgram.Activate();
		glUniform1f(uniID,1.0f);
		glBindTexture(GL_TEXTURE_2D, 0);
		//Binds vertex array over and over again
		glBindVertexArray(VAO);
		//Does the thing to draw the array
		glDrawElements(GL_TRIANGLES, 9 , GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(win);

	}
	
	//Cleans up 
	std::cout << "Cleaning up arrays and buffers\n";
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO_pos);
	glDeleteTextures(1, &texture);
	shaderProgram.Delete();
	//Kills the window lol, cleans up resources I guess
	glfwDestroyWindow(win);
	//Ends glfw stuff, I guess?
	glfwTerminate();

	return 0;
}
