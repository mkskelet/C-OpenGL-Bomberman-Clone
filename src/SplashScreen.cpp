///
/// SplashScreen scene class. Renders splash screen and requests change to main menu scene.
///

#include "SplashScreen.h"
#include <iostream>

/// Constructor for main menu scene, sets player count and ai count to lowest needed for the game to start.
SplashScreen::SplashScreen() : Scene(SCENE_SPLASHSCREEN) {
	OnWindowResized();
}

/// Function to render scene.
void SplashScreen::Render(){
	background.Render();
	//madeBy.Render();
	
	glColor3f(0.7f,0.0f,1.0f);
	glRasterPos2f(0, 0);
	int len, i;
	char* arr = (char*) "Made by Marek Kost\0";
	len = (int)strlen(arr);
	for(i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, arr[i]);
	}
	glColor3f(1.0f,1.0f,1.0f);
}

/// Method called when scene is created.
void SplashScreen::Start() {
	timer = GameTime::GetTime() + 2000;	// set timer to 2000 miliseconds after current time
	
	int err;
	if((err = TGA::loadTGA((char*)"src/sprites/splashscreen.tga", 1)) != 1)
		std::cout << "ERROR " << err;
}

/// Function to update scene.
void SplashScreen::Update() {
	if(timer <= GameTime::GetTime()) {
		SwitchScene(SCENE_MAIN_MENU);
		timer = 1000000000;				// dont want to repeatedly call switch scene
	}
}

/// Method called when key is pressed.
void SplashScreen::KeyPress(unsigned char key) {
	if(key == 27) {
		exit = true;
	}
	else SwitchScene(SCENE_MAIN_MENU);
}

/// Method called when special key is pressed.
void SplashScreen::SpecialKeyPress(int key) {
	SwitchScene(SCENE_MAIN_MENU);
}

/// Method called after window has been resized
void SplashScreen::OnWindowResized() {
	float w = (float)glutGet(GLUT_WINDOW_WIDTH);
	float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
	
	background = Sprite(Vector2(0, h), Vector2(w, h), 1, PIVOT_TOPLEFT);
	madeBy = Sprite(Vector2(5,5+h/20), Vector2(h/5, h/20), 0, PIVOT_TOPLEFT);
}