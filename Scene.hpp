#include <string	>


class Scene {
public:
	Scene();
	virtual bool loop();
	virtual void draw();
	virtual ~Scene();
};

void loadScene(Scene* scene);