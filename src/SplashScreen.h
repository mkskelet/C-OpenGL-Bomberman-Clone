///
/// Splash screen scene class. Renders splash screen animation and changes scene to main menu.
///

#ifndef SplashScreen_h
#define SplashScreen_h

#include "Scene.h"
#include "GameTime.h"
#include "tga.h"
#include "Sprite.h"

class SplashScreen: public Scene{
private:
	long timer;			///< Time before switching scene.
	Sprite background;		///< Background sprite.
	Sprite madeBy;
public:
	SplashScreen();
	void Render();
	void Start();	
	void Update();
	void KeyPress(unsigned char key);
	void SpecialKeyPress(int key);
	void OnWindowResized();
};

#endif // SplashScreen_h