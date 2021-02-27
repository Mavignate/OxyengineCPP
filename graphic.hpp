#pragma once
#include <GL/glew.h>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include"Event.hpp"	
    
    SDL_Color toSDLColorBGRA(glm::vec4 color);
    
    template <typename T>
	void glBufferDataV(GLenum Target, const std::vector<T>& buffer, GLenum type) {
		glBufferData(Target, sizeof(T) * buffer.size(), buffer.data(), type);
	}

	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texture;
	};

	class Shader
	{
	public:
		Shader(std::string FileVertexShader, std::string FileFragmentShader);
		~Shader();
		GLuint getFS();
		GLuint getVS();
		GLuint getprogram();
		void loadNewProgram(std::string FileVertexShader, std::string FileFragmentShader);
		void addComponent();
		bool CanDestroy();
		void use();
	private:
		int usedByComponent;
		GLuint fragmentshader;
		GLuint vertexshader;
		GLuint program;
		void shaderload(std::string vs, std::string fs);
		void linkprogram();
	};

	class Mesh {

	public:
		Mesh(std::vector <Vertex>* VertexArray, std::vector <GLuint>* ElementBuff);
		void draw();
		void setVertex(glm::vec2 Position, glm::vec2 size, glm::vec2 normalizevec);
		~Mesh();
	private:

		GLuint ProgramPosition;
		GLuint ProgramColor;
		GLuint ProgramTexture;
		GLuint vbo;
		GLuint ebo;
	};

	class Texture
	{
	public:
		Texture(const char* filename, bool mipmap = true);
		Texture(SDL_Surface* surface, bool mipmap = true);
		GLuint GetTexture();
	
		void use();
		float GetWidth();
		float GetHeight();
		glm::vec2 normalizevec;
		~Texture();
	private:
		bool load(SDL_Surface* sur, bool mipmap);

		float w, h;
		GLuint texture;
	};