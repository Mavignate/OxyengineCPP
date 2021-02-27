#include "graphic.hpp"
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <fstream>	
#include <cstdlib>




std::string loadfile(std::string File) {
	std::fstream file;
	std::string out;
	file.open(File, std::ios::in);
	if (!file.good()) {
		std::cerr << "failed open: " << File;
		exit(0);
	}
	std::string line;
	while (getline(file, line))
	{
		out += line + "\n";
	}
	return out;
}

Shader::Shader(std::string FileVertexShader, std::string FileFragmentShader)
{

	shaderload(FileVertexShader, FileFragmentShader);
	linkprogram();

}
Shader::~Shader()
{
}
GLuint Shader::getFS()
{
	return fragmentshader;
}
GLuint Shader::getVS()
{
	return vertexshader;
}
GLuint Shader::getprogram()
{
	return program;
}
void Shader::loadNewProgram(std::string vs, std::string fs)
{
	std::string str = loadfile(vs);
	const char* ShaderText = str.c_str();
	GLint sucess;
	GLchar errbufer[512];
	glShaderSource(vertexshader, 1, &ShaderText, NULL);
	glCompileShader(vertexshader);
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &sucess);
	if (!sucess) {
		glGetShaderInfoLog(vertexshader, 512, NULL, errbufer);
		std::cerr << "faile to compiled shader from file: " << vs << " " << errbufer;
	}

	str = loadfile(fs);
	ShaderText = str.c_str();

	glShaderSource(fragmentshader, 1, &ShaderText, NULL);
	glCompileShader(fragmentshader);
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &sucess);
	if (!sucess) {
		glGetShaderInfoLog(fragmentshader, 512, NULL, errbufer);
		std::cerr << "failed to compile Shader from file: " << fs << " " << errbufer;
	}
	glAttachShader(program, vertexshader);
	glAttachShader(program, fragmentshader);
	glUseProgram(program);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &sucess);
	if (!sucess) {
		glGetProgramInfoLog(program, 512, NULL, errbufer);
		std::cerr << "Error link program" << errbufer;
	}
	glDetachShader(program, vertexshader);
	glDetachShader(program, fragmentshader);
}
void Shader::addComponent()
{
	usedByComponent++;
}
bool Shader::CanDestroy()
{
	return usedByComponent <= 0;
}
void Shader::use()
{
	glUseProgram(program);
}
void Shader::shaderload(std::string vs, std::string fs)
{
	std::string str = loadfile(vs);
	const char* ShaderText = str.c_str();
	GLint sucess;
	GLchar errbufer[512];
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexshader, 1, &ShaderText, NULL);
	glCompileShader(vertexshader);
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &sucess);
	if (!sucess) {
		glGetShaderInfoLog(vertexshader, 512, NULL, errbufer);
		std::cerr << "faile to compiled shader from file: " << vs << " " << errbufer;
	}
	str = loadfile(fs);
	ShaderText = str.c_str();

	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentshader, 1, &ShaderText, NULL);
	glCompileShader(fragmentshader);
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &sucess);
	if (!sucess) {
		glGetShaderInfoLog(fragmentshader, 512, NULL, errbufer);
		std::cerr << "failed to compile Shader from file: " << fs << " " << errbufer;
	}
}
void Shader::linkprogram()
{
	program = glCreateProgram();
	glAttachShader(program, vertexshader);
	glAttachShader(program, fragmentshader);

	glLinkProgram(program);
	glUseProgram(program);
	GLint sucess;
	GLchar errbufer[512];
	glGetProgramiv(program, GL_LINK_STATUS, &sucess);
	if (!sucess) {
		glGetProgramInfoLog(program, 512, NULL, errbufer);
		std::cerr << "Error link program" << errbufer;
	}

	glDetachShader(program, vertexshader);
	glDetachShader(program, fragmentshader);
}

Mesh::Mesh(std::vector<Vertex>* VertexArray, std::vector<GLuint>* ElementBuff)
{

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferDataV(GL_ARRAY_BUFFER, *VertexArray, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferDataV(GL_ELEMENT_ARRAY_BUFFER, *ElementBuff, GL_DYNAMIC_DRAW);
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	ProgramPosition = glGetAttribLocation(id, "Position");
	glVertexAttribPointer(ProgramPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(ProgramPosition);
	ProgramColor = glGetAttribLocation(id, "Color");
	glVertexAttribPointer(ProgramColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(ProgramColor);
	ProgramTexture = glGetAttribLocation(id, "Texture");
	glVertexAttribPointer(ProgramTexture, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture));
	glEnableVertexAttribArray(ProgramTexture);
	delete VertexArray;
	delete ElementBuff;
}

void Mesh::draw()
{
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}


Mesh::~Mesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

Texture::Texture(const char* filename, bool mipmap)
{
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* img = IMG_Load(filename);
	if(img == nullptr) {
		printf("Error: %s", IMG_GetError());
		fflush(stdout);
	}
	load(img, mipmap);
	IMG_Quit();
}

Texture::Texture(SDL_Surface* surface, bool mipmap)
{
	load(surface, mipmap);
}

GLuint Texture::GetTexture()
{
	return texture;
}

void Mesh::setVertex(glm::vec2 Position, glm::vec2 size, glm::vec2 normalizevec)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
		 Vertex rectvetex[] = {
	{glm::vec3(-size.x/2, size.y/2, 0.0f), glm::vec4(), glm::vec2(Position.x * normalizevec.x, (Position.y + size.y) * normalizevec.y)},
	{glm::vec3(size.x/2, size.y/2, 0.0f), glm::vec4(), glm::vec2((Position.x + size.x) * normalizevec.x, (Position.y + size.y) * normalizevec.y) },
	{glm::vec3(size.x/2, -size.y/2, 0.0f), glm::vec4(),glm::vec2((Position.x + size.x) * normalizevec.x, Position.y * normalizevec.y)},
	{glm::vec3(-size.x/2, -size.y/2, 0.0f), glm::vec4(), Position * normalizevec}
		};
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(rectvetex), rectvetex);
}

void Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

float Texture::GetWidth()
{
	return w;
}

float Texture::GetHeight()
{
	return h;
}



Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

bool Texture::load(SDL_Surface* sur, bool mipmap)
{
	w = sur->w;
	h = sur->h;
	normalizevec.x = 1 / w;
	normalizevec.y = 1 / h;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (!mipmap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, sur->w, sur->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sur->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, NULL);
	SDL_FreeSurface(sur);
	return true;

}

