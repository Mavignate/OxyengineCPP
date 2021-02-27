#include "graphic.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

void setVideoMode(float w, float h);

class Object{
public:
    Object();
    virtual void draw() = 0;
    void setPos(glm::vec2 Pos);
    void scale(glm::vec2 size);
    void rotate(float angle);
    glm::vec2 getPosiotion();
    glm::vec2 getSize();
    float getAngle();
    ~Object();
protected:
    glm::vec2 Position = glm::vec2(0, 0);
    glm::vec2 size = glm::vec2(1, 1);
    float angle = 0;
    glm::mat4 transform = glm::mat4(1);
};

struct Style
{
    glm::vec4 borderColor;
    glm::vec4 backGroundColor;
    glm::vec4 textColor;
    int border;
    int padding;
    TTF_Font* font;
    std::shared_ptr<Event> event;
    std::shared_ptr<Shader> shader;
};

