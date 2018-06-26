///
/// main.cpp
/// Main function of game. Creates Game window and initializes openGL.
///

#include <GL/glut.h>
#include "GameWindow.h"
#include "GameTime.h"
#include "PlayerPrefs.h"
#include <iostream>
using namespace std;

#define SMALLEST_WIDTH 1
#define SMALLEST_HEIGHT 1

GameWindow gameWindow;
int wid = 0;		///< window id
int windowX = SMALLEST_WIDTH;
int windowY = SMALLEST_HEIGHT;
int windowPosX = 100;
int windowPosY = 100;

void RenderFunc() {
	gameWindow.Render();
}

void UpdateFunc() {
	if(gameWindow.exit) {
		glutDestroyWindow(wid);
		exit(0);
	}
	
	gameWindow.Update();
}

void KeyboardFunc(unsigned char key, int x, int y) {
	gameWindow.KeyPress(key);
}

void KeyboardUpFunc(unsigned char key, int x, int y) {
	gameWindow.KeyRelease(key);
}

void SpecialFunc(int key, int x, int y) {
	gameWindow.SpecialKeyPress(key);
}

void SpecialUpFunc(int key, int x, int y) {
	gameWindow.SpecialKeyRelease(key);
}

void TimerFunc(int t) {
	GameTime::AddTime(t);
	glutTimerFunc(t, TimerFunc, t);
	
	// check if window didnt move
	if(windowPosX != glutGet(GLUT_WINDOW_X) || windowPosY != glutGet(GLUT_WINDOW_Y)) {
		PlayerPrefs::SetInt("WindowPosX", glutGet(GLUT_WINDOW_X));
		PlayerPrefs::SetInt("WindowPosY", glutGet(GLUT_WINDOW_Y));
	}
	
	if(gameWindow.exit) {
		glutDestroyWindow(wid);
		exit(0);
	}
	
	gameWindow.Update();
	//glutPostRedisplay();
}

/// Function resizes window if it is smaller than smallest allowed size.
void Resize(int width, int height) {
	if(width != windowX || height != windowY ) {
		windowX = width < SMALLEST_WIDTH ? SMALLEST_WIDTH : width;
		windowY = height < SMALLEST_HEIGHT ? SMALLEST_HEIGHT : height;
		
		// resize window
		glutReshapeWindow(windowX, windowY);
		
		// set orthographic view
		glViewport(0,0,windowX,windowY);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, windowX, 0, windowY);
		glMatrixMode(GL_MODELVIEW);
		
		// tell game about windows size change
		gameWindow.OnWindowResized();
		
		RenderFunc();
		
		PlayerPrefs::SetInt("WindowX", windowX);
		PlayerPrefs::SetInt("WindowY", windowY);
		PlayerPrefs::SetInt("WindowPosX", glutGet(GLUT_WINDOW_X));
		PlayerPrefs::SetInt("WindowPosY", glutGet(GLUT_WINDOW_Y));
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	windowX = PlayerPrefs::GetInt("WindowX", 480);
	windowY = PlayerPrefs::GetInt("WindowY", 480);
	windowPosX = PlayerPrefs::GetInt("WindowPosX", 100);
	windowPosY = PlayerPrefs::GetInt("WindowPosY", 100);
	glutInitWindowSize(windowX,windowY);
	glutInitWindowPosition(windowPosX,windowPosY);
	wid = glutCreateWindow("Bomberman");
	
	gameWindow.Start();
	
	glutDisplayFunc(RenderFunc);
	//glutIdleFunc(UpdateFunc); zere CPU jak svina, pouzivat timer s pevne definovanym poctom tikov za sekundu
	glutKeyboardFunc(KeyboardFunc);
	glutKeyboardUpFunc(KeyboardUpFunc);
	glutSpecialFunc(SpecialFunc);
	glutSpecialUpFunc(SpecialUpFunc);
	glutReshapeFunc(Resize);
	glutTimerFunc(1000/30, TimerFunc, 1000/30);
	glDisable(GL_DEPTH_TEST);
	
	// set orthographic view
	glViewport(0,0,windowX,windowY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, windowX, 0, windowY);
	glMatrixMode(GL_MODELVIEW);
	
	glutMainLoop();
	
	return 0;
}