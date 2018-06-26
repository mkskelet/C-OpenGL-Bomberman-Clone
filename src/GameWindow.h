///
/// GameWindow.h
/// Class takes care of rendering and updating everything inside game window.
///

#ifndef GameWindow_h
#define GameWindow_h

#include <GL/glut.h>
#include "MainMenu.h"
#include "SplashScreen.h"
#include "Level.h"

class GameWindow {
private:
	int currentScene;
	Scene *scene;
public:
	bool exit;
	GameWindow();
	~GameWindow();
	void Start();
	void Render();
	void Update();
	void KeyPress(unsigned char key);
	void KeyRelease(unsigned char key);
	void SpecialKeyPress(int key);
	void SpecialKeyRelease(int key);
	void SwitchScene(int sceneIndex);
	void OnWindowResized();
};

#endif // GameWindow_h