#include "shaderClass.h"

std::string get_file_contents(const char *filename){
	std::ifstream in(filename, std::ios::binary);
	if(in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char *vertexFile, const char *fragmentFile){
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char *vertexSource = vertexCode.c_str();
	const char *fragmentSource = vertexCode.c_str();
	// This creates the vertex shader object and loads in the code
	// that was at the start of this file
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertexSource, NULL);
	glCompileShader(vertex_shader);

	//Loads in the fragment shader object, and once again, puts in the code
	// that we saw at the beginning of the file, puts it in a fragment shader object
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragmentSource, NULL);
	glCompileShader(fragment_shader);

	//Shader program
		//Creates a shader program to use the shaders
		ID = glCreateProgram();

		//Attaches the vertex shader to the shader program
		glAttachShader(ID, vertex_shader);
		//Attaches fragment shader to shader program
		glAttachShader(ID, fragment_shader);

		//Links program, so it compiles and runs with the rest of the code I guess?
		glLinkProgram(ID);

		//Now that we've loaded the shaders into the shader program, we can delete the shader objects
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

	//Shader program end
}

void Shader::Activate(){
	glUseProgram(ID);
}

void Shader::Delete(){
	glDeleteProgram(ID);
}
