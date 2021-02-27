#include "Object.hpp"
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <fstream>	
#include <cstdlib>


glm::mat4 display = glm::mat4(1);


void setVideoMode(float w, float h) {
	display = glm::ortho(0.0f, w, h, 0.f, 0.0f, 1.f);
}

Object::Object()
{
}

void Object::draw()
{
	transform = glm::mat4(1);
	transform = glm::translate(transform, glm::vec3(Position.x, Position.y, 0.f));
	transform = glm::scale(transform, glm::vec3(size.x, size.y, 0.f));
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	glUniformMatrix4fv(glGetUniformLocation(id, "transform"), 1, GL_FALSE, value_ptr(transform));
	glUniformMatrix4fv(glGetUniformLocation(id, "camera"), 1, GL_FALSE, value_ptr(display));
}

void Object::setPos(glm::vec2 Pos)
{
	Position = Pos;
}

void Object::scale(glm::vec2 size)
{
	this->size = size;
}

void Object::rotate(float angle)
{
	this->angle = angle;
}

glm::vec2 Object::getPosiotion()
{
	return Position;
}

glm::vec2 Object::getSize()
{
	return size;
}

float Object::getAngle()
{
	return angle;
}

Object::~Object()
{
}
