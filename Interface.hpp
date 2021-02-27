#include "Object.hpp"
#include "graphic.hpp"


class Image
    :public Object
{
public:
    Image( std::string&& fileImage, bool mipmap, Style& style);
    void draw();
    ~Image();
private:
    std::shared_ptr<Texture> img;
    std::unique_ptr<Mesh> rect;
    std::shared_ptr<Shader> shader;
};

class Text
    :public Object
{
public:
    Text(std::string&& text, Style& style );
    void setText(std::string text, glm::vec4 color);
    std::string getText();
    void draw();
    ~Text();

private:
    std::string text;
    TTF_Font* font;
    std::unique_ptr<Texture> img;
    std::unique_ptr<Mesh> rect;
    std::shared_ptr<Shader> shader;
};

class Button
    :public Object
{
public:
    Button(std::string&& text, Style& style);
    bool clicked();
    void draw();
    ~Button();

private:
    std::string text;
    TTF_Font* font;
    std::unique_ptr<Texture> img;
    std::unique_ptr<Mesh> rect;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Event> event;
};

class List
    :public Object
{
public:
    List(std::vector <std::string>&& Texts, Style& style);
    int getActive();
    virtual void draw();
    ~List();

private:
    glm::vec2 Element;
    float activeTexture;
    float activeElement = 0;
    std::vector <std::string> Text;
    TTF_Font* font;
    std::unique_ptr<Texture> img;
    std::unique_ptr<Mesh> rect;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Event> event;
};
