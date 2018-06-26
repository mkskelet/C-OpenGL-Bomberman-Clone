///
/// GameWindow.cpp
/// Class takes care of rendering and updating everything inside game window.
///

#include "GameWindow.h"
#include "MapLoader.h"

GameWindow::GameWindow() : exit(false){
	
}

/// Method should be called after initializing opengl. Method switches scene to splash screen.
void GameWindow::Start() {
	SwitchScene(SCENE_SPLASHSCREEN);
	
	// TEMP CODE FOR LOADING MAP, DELETE L8R
	//MapLoader mapLoader;
	//Map m = mapLoader.LoadMap(mapLoader.GetMapList(true)[0]);
}

GameWindow::~GameWindow() {
	delete scene;
}

void GameWindow::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	// drawing
	scene->Render();
	
	glutSwapBuffers();
}

/// Calls update function for all the objects in game by calling scene update function.
void GameWindow::Update() {
	// updates
	scene->Update();
	
	// check if we need to switch scene
	if(scene->IsSwitchingScene())
		SwitchScene(scene->GetNextScene());
	if(scene->exit)
		exit = true;
	
	glutPostRedisplay();
}

/// Method called when key was pressed.
void GameWindow::KeyPress(unsigned char key) {
	scene->KeyPress(key);
}

/// Method called when special key was pressed.
void GameWindow::SpecialKeyPress(int key) {
	scene->SpecialKeyPress(key);
}

void GameWindow::KeyRelease(unsigned char key) {
	scene->KeyRelease(key);
}

void GameWindow::SpecialKeyRelease(int key) {
	scene->SpecialKeyRelease(key);
}

/// Method switches the scene to scene with index of sceneIndex parameter.
void GameWindow::SwitchScene(int sceneIndex) {
	delete scene;
	currentScene = sceneIndex;
	
	switch(sceneIndex) {
		case SCENE_SPLASHSCREEN:
			scene = new SplashScreen();
			break;
		case SCENE_MAIN_MENU:
			scene = new MainMenu();
			break;
		case SCENE_LEVEL:
			scene = new Level();
			break;
		default:
			scene = new SplashScreen();
			break;
	}
	
	scene->Start();
}

/// Method called after window has been resized
void GameWindow::OnWindowResized() {
	scene->OnWindowResized();
}