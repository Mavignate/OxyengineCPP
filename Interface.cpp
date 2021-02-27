#include <GL/glew.h>
#include "Interface.hpp"
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>	
#include <cstdlib>


bool hover(std::shared_ptr<Event> event, glm::vec2 Position, glm::vec2 size) {
	glm::vec2 upWall = glm::vec2(Position.x - (size.x / 2), Position.x + (size.x / 2));
	glm::vec2 downWall = glm::vec2(Position.y - (size.y / 2), Position.y + (size.y / 2));
	return event->mousePos.x >= upWall.x &&
		event->mousePos.x <= upWall.y &&
		event->mousePos.y >= downWall.x &&
		event->mousePos.y <= downWall.y;
}


Mesh* CreateRect(glm::vec2 size) {
	float w = size.x / 2;
	float h = size.y / 2;
	Vertex rectvetex[] = {
	{glm::vec3(-w, h, 0.0f), glm::vec4(), glm::vec2(0,1)},
	{glm::vec3(w, h, 0.0f), glm::vec4(), glm::vec2(1,1)},
	{glm::vec3(w, -h, 0.0f), glm::vec4(), glm::vec2(1,0)},
	{glm::vec3(-w, -h, 0.0f), glm::vec4(), glm::vec2(0,0)}
	};
	static GLuint indi[] = { 0, 1, 2, 2, 3, 0 };
return new Mesh(new std::vector <Vertex>(rectvetex, std::end(rectvetex)), new std::vector <GLuint>(indi, std::end(indi)));
}

Image::Image(std::string&& fileImage, bool mipmap, Style& style)
	:shader(style.shader), img(new Texture(fileImage.c_str(), mipmap)), rect(CreateRect(glm::vec2(img->GetWidth(), img->GetHeight())))
{
}

void Image::draw()
{
	shader->use();
	Object::draw();
	img->use();
	rect->draw();
}

Image::~Image()
{
}

Text::Text(std::string&& text, Style& style)
	:shader(style.shader)
{
	this->text = std::move(text);
	this->font = style.font;
	if (font == nullptr)
	{
		std::cerr << "Error font" << std::endl;
	}
	else
	{

		SDL_Surface* genText = TTF_RenderText_Blended(font, this->text.c_str(), toSDLColorBGRA(style.textColor));
		if (genText == NULL)
		{

			printf("Oh My Goodness, an error : %s", TTF_GetError());
		}

		img.reset(new Texture(genText));
		SDL_FreeSurface(genText);
		if (img == nullptr)
		{
			std::cerr << "error create texture with text and font" << std::endl;
		}
		else
		{
			rect.reset(CreateRect(glm::vec2(img->GetWidth(), img->GetHeight())));
		}

	}


}

void Text::setText(std::string text, glm::vec4 color)
{
	this->text = text;
	if (font == nullptr)
	{
		std::cerr << "Error font" << std::endl;
	}
	else
	{

		SDL_Surface* genText = TTF_RenderText_Blended(font, this->text.c_str(), toSDLColorBGRA(color));
		img.reset(new Texture(genText));
		SDL_FreeSurface(genText);
		if (img == nullptr)
		{
			std::cerr << "error create texture with text and font" << std::endl;
		}
		else
		{
			rect.reset(CreateRect(glm::vec2(img->GetWidth(), img->GetHeight())));
		}
	}
}

std::string Text::getText()
{
	return text;
}

void Text::draw()
{
	shader->use();
		Object::draw();
	img->use();
	rect->draw();
}

Text::~Text()
{
}

SDL_Color toSDLColorBGRA(glm::vec4 color)
{
	SDL_Color convert;
	convert.r = color.z;
	convert.g = color.y;
	convert.b = color.x;
	convert.a = color.w;
	return convert;
}

SDL_Surface* drawRectBorderInSurface(int w, int h, int border, glm::vec4 borColor, glm::vec4 Color) {
	SDL_Rect square;
	square.w = w + border ;
	square.h = h + border ;
	SDL_Surface* gen = SDL_CreateRGBSurface(SDL_SWSURFACE, square.w, square.h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	SDL_FillRect(gen, NULL, SDL_MapRGBA(gen->format, borColor.x, borColor.y, borColor.z, Color.w));
	square.w -= border;
	square.h -= border;
	square.x = border;
	square.y = border;
	SDL_Surface* paddingSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, square.w, square.h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	SDL_FillRect(paddingSurface, NULL, SDL_MapRGBA(paddingSurface->format, Color.x, Color.y, Color.z, Color.w));
	square.x = border / 2;
	square.y = square.x;
	square.w = NULL;
	square.h = NULL;
	SDL_BlitSurface(paddingSurface, NULL, gen, &square);
	SDL_FreeSurface(paddingSurface);
	return gen;
}

Button::Button(std::string&& text,Style& style)
	:shader(style.shader), font(style.font), event(style.event)
{
	this->text = std::move(text);
	if (font == nullptr)
	{
		std::cerr << "Error font" << std::endl;
	}
	else
	{

		SDL_Surface* genText = TTF_RenderText_Blended(font, this->text.c_str(), toSDLColorBGRA(style.textColor));
		if (genText == NULL)
		{
			printf("Oh My Goodness, an error : %s", TTF_GetError());
		}
		SDL_Rect square;
		square.w = genText->w + style.padding;
		square.h = genText->h + style.padding;
		SDL_Surface* gen = drawRectBorderInSurface(square.w, square.h, style.border, style.borderColor, style.backGroundColor);
		square.x = (style.border + style.padding) / 2;
		square.y = square.x;
		SDL_BlitSurface(genText, NULL, gen, &square);
		img.reset(new Texture(gen, false));
		SDL_FreeSurface(genText);
		SDL_FreeSurface(gen);
		if (img == nullptr)
		{
			std::cerr << "error create texture with text and font" << std::endl;
		}
		else
		{
			rect.reset(CreateRect(glm::vec2(img->GetWidth(), img->GetHeight())));

		}

	}
}

bool Button::clicked()
{
	if (hover(event, Position, glm::vec2(img->GetWidth(), img->GetHeight())) && event->mouseClick() == 1)
	{
		return true;
	}
	return false;
}

void Button::draw()
{
	shader->use();
	Object::draw();
	img->use();
	if (hover(event,Position,glm::vec2(img->GetWidth(),img->GetHeight()))) {
		scale(glm::vec2(1.25,1.25));
	}
	else
	{
		scale(glm::vec2(1, 1));
	}
	rect->draw();

}

Button::~Button()
{
}



List::List(std::vector<std::string>&& Texts, Style& style)
	:shader(style.shader), font(style.font), event(style.event)
{
	this->Text = std::move(Texts);
	for (int i = 0; i < this->Text.size(); i++)
	{
		SDL_Surface* text = TTF_RenderText_Blended(font, Text[i].c_str() , toSDLColorBGRA(style.textColor));
		if (Element.x < text->w) Element.x = text->w; Element.y = text->h/2;
		SDL_FreeSurface(text);
	}
	int textpos = style.border + style.padding;
	SDL_Surface* Button = drawRectBorderInSurface(Element.x + (2 * textpos), Element.y + (2 * textpos), style.border, style.borderColor, style.backGroundColor);
	
	SDL_Surface* gen = SDL_CreateRGBSurface(SDL_SWSURFACE, Button->w, Button->h * this->Text.size(), 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

	for (int i = 0; i < this->Text.size(); i++)
	{
		SDL_Rect pos;
		SDL_Surface* text = TTF_RenderText_Blended(font, Text[i].c_str(), toSDLColorBGRA(style.textColor));
	
		pos.y = Button->h * i;
		pos.x = 0;
		SDL_BlitSurface(Button, NULL, gen, &pos);
		pos.y += textpos / 2;
		pos.x = textpos / 2;
		SDL_BlitSurface(text, NULL, gen, &pos);
		SDL_FreeSurface(text);
	}
	SDL_FreeSurface(Button);
	img.reset(new Texture(gen, false));
	rect.reset(CreateRect(glm::vec2(img->GetWidth(), img->GetHeight())));
	Element.y = img->GetHeight() / this->Text.size();
	Element.x = img->GetWidth();
	rect->setVertex(glm::vec2(0, 0), Element, img->normalizevec);
}

int List::getActive()
{
	return activeElement;
}

void List::draw()
{
	shader->use();
		Object::draw();

	

	if (hover(event,Position,Element))
	{
		
		scale(glm::vec2(1.2,1.2));
	}
	else
	{
		scale(glm::vec2(1, 1));
	}

	if (hover(event, Position, Element) && event->mouseClick() == 1) {
		scale(glm::vec2(1, 1));
		activeElement++;
		rect->setVertex(glm::vec2(0, Element.y*activeElement), Element, img->normalizevec);
	}


	img->use();
	rect->draw();
}

List::~List()
{
}
