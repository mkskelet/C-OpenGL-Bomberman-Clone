///
/// Scene.cpp
/// Base class for all the scenes in game. (MainMenu and Level)
///

#include "Scene.h"

#define SCENE_SPLASHSCREEN 0
#define SCENE_MAIN_MENU 1
#define SCENE_LEVEL 2

Scene::Scene(int index):
	switchScene(false),
	nextScene(0),
	sceneIndex(index),
	exit(false){
	
}

/// Returns index of next scene.
int Scene::GetNextScene() {
	return nextScene;
}

/// Method called when redraw is needed.
void Scene::Render() {
	
}

/// Method called every frame.
void Scene::Update() {
	
}

/// Method called when scene is loaded.
void Scene::Start() {
	
}

/// Method called when key is pressed.
void Scene::KeyPress(unsigned char key) {
	
}

/// Method called when a special key is pressed.
void Scene::SpecialKeyPress(int key) {
	
}

/// Method called when we need to switch scene.
void Scene::SwitchScene(int newIndex) {
	nextScene = newIndex;
	switchScene = true;
}

/// Self explanatory.
bool Scene::IsSwitchingScene() {
	return switchScene;
}

/// Method called after window has been resized
void Scene::OnWindowResized() {
	
}

void Scene::KeyRelease(unsigned char key) {
	
}

void Scene::SpecialKeyRelease(int key) {
	
}