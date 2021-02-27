#include "Window.hpp"
#include "GameMenu.hpp"

#define Setting "OxySetting.conf"

void createWindowWithSetting(std::string file);

#ifdef main
#undef main
#endif

int main() {
	createWindowWithSetting(Setting);
	//setVideoMode(800, 600);
	//Scene* menu = new GameMenu();
	loadScene(nullptr);
	SDL_Quit();
	return 0;
}

void createWindowWithSetting(std::string file)
{
}