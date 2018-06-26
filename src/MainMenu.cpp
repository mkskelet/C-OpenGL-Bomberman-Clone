///
/// MainMenu.cpp
/// Main menu scene class. Renders and controls main menu behavior.
///

#include "MainMenu.h"
#include <iostream>
#include "Level.h"
#include "PlayerPrefs.h"
#include <sstream>

using namespace std;

// menu texture IDs 2-10
#define MAINMENU_BACKGROUND 2
#define MAINMENU_1PLAYER_NORMAL 3
#define MAINMENU_2PLAYERS_NORMAL 4
#define MAINMENU_EXIT 5
#define MAINMENU_CONTROLS 6
#define LEVELSELECT_NEXT 7
#define LEVELSELECT_BACK 8
#define LEVELSELECT_MENU 9
#define MAINMENU_CONTROLS_BOX 10

//block texture IDs 11-15
#define PREVIEW_BLOCK_BACKGROUND 11
#define PREVIEW_BLOCK_SOLID 12
#define PREVIEW_BLOCK_EXPLODABLE 13
#define PREVIEW_BLOCK_BOMBERMAN 14
#define PREVIEW_BLOCK_CREEP 15
#define PREVIEW_BLOCK_BOMB 16
#define PREVIEW_BLOCK_EXPLOSION 17
#define PREVIEW_BLOCK_PORTAL 18

/// Constructor for main menu scene, sets player count and ai count to lowest needed for the game to start.
MainMenu::MainMenu() : Scene(SCENE_MAIN_MENU),
					showMainMenu(true),
					previewMapIndex(0),
					singlePlayer(true),
					showControls(false),
					menuPosition(SINGLE_PLAYER),
					mapName("")					{
	// initializing sprites, all this stuff has to be in OnWindowResized() function if we wanna have responsive design
	OnWindowResized();
}

/// Function to render scene.
void MainMenu::Render(){
	// draw background
	background.Render();
	
	if(showMainMenu) {
		// draw 1 player button
		if(menuPosition == SINGLE_PLAYER)
			spButton.SetColor(0.8f,0,0);
		else spButton.SetColor(0.8f,0.8f,0);
		
		spButton.Render();
		
		// draw 2 players button
		if(menuPosition == MULTI_PLAYER)
			mpButton.SetColor(0.8f,0,0);
		else mpButton.SetColor(0.8f,0.8f,0);
		
		mpButton.Render();
		
		// draw controls button
		if(menuPosition == MENU_CONTROLS)
			controlsButton.SetColor(0.8f,0,0);
		else controlsButton.SetColor(0.8f,0.8f,0);
		
		controlsButton.Render();
		
		// draw controls box
		if(showControls)
			controlsBox.Render();
		
		// draw exit button
		if(menuPosition == MENU_EXIT)
			exitButton.SetColor(0.8f,0,0);
		else exitButton.SetColor(0.8f,0.8f,0);
		
		exitButton.Render();
	}
	else {
		// print map name
		float w = (float)glutGet(GLUT_WINDOW_WIDTH);
		float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
		
		glColor3f(0.7f,0.0f,1.0f);
		glRasterPos2f(w/2-90, h/4*3+25);
		{
			const char* arr;
			if(mapLoader.GetMapList(singlePlayer).size() > previewMapIndex)
				arr = mapLoader.GetMapList(singlePlayer)[previewMapIndex].c_str();
			else arr = (char*) "NO MAP FOUND.";
			int len = (int)strlen(arr);
			for(int i = 0; i < len; i++) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, arr[i]);
			}
		}
		glColor3f(1.0f,1.0f,1.0f);
		
		// print high score
		stringstream ss;
		glColor3f(0.7f,0.0f,1.0f);
		glRasterPos2f(w/2-90, h/4*3+5);
		if(mapLoader.GetMapList(singlePlayer).size() > previewMapIndex && PlayerPrefs::GetInt(mapLoader.GetMapList(singlePlayer)[previewMapIndex], -1) != -1) {
			ss << PlayerPrefs::GetInt(mapLoader.GetMapList(singlePlayer)[previewMapIndex], -1);
			string score = ss.str();
			const char* arr1 = score.c_str();
			int len = (int)strlen(arr1);
			for(int i = 0; i < len; i++) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, arr1[i]);
			}
		}
		glColor3f(1.0f,1.0f,1.0f);
		
		// print map
		if(mapPreview.size() > 0) {
			for(int i = 0; i < mapPreview.size(); i++)
				mapPreview[i].Render();
		}
		
		// print navigation arrows
		if(previewMapIndex == 0) arrowPrev.SetTextureID(LEVELSELECT_MENU);
		else arrowPrev.SetTextureID(LEVELSELECT_BACK);
		arrowPrev.Render();
		
		if(mapLoader.GetMapList(singlePlayer).size() > previewMapIndex+1)
			arrowNext.Render();
	}
}

/// Method called when scene is created. 
void MainMenu::Start() {
	int err;
	if((err = TGA::loadTGA((char*)"src/sprites/menu/screen.tga", MAINMENU_BACKGROUND)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/menu/mm_1pl.tga", MAINMENU_1PLAYER_NORMAL)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/menu/mm_2pl.tga", MAINMENU_2PLAYERS_NORMAL)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/blocks/BackgroundTile.tga", PREVIEW_BLOCK_BACKGROUND)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/blocks/SolidBlock.tga", PREVIEW_BLOCK_SOLID)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/blocks/ExplodableBlock.tga", PREVIEW_BLOCK_EXPLODABLE)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/blocks/Bman_F_f00.tga", PREVIEW_BLOCK_BOMBERMAN)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/blocks/Creep_F_f00.tga", PREVIEW_BLOCK_CREEP)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/menu/mm_exit.tga", MAINMENU_EXIT)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/menu/mm_ctrl.tga", MAINMENU_CONTROLS)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/menu/mm_ctrlbox.tga", MAINMENU_CONTROLS_BOX)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/menu/ls_back.tga", LEVELSELECT_BACK)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/menu/ls_menu.tga", LEVELSELECT_MENU)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/menu/ls_next.tga", LEVELSELECT_NEXT)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/blocks/Bomb.tga", PREVIEW_BLOCK_BOMB)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/blocks/Flame.tga", PREVIEW_BLOCK_EXPLOSION)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/blocks/Portal.tga", PREVIEW_BLOCK_PORTAL)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/blocks/BP.tga", 19)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/blocks/FP.tga", 20)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/blocks/SP.tga", 21)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/blocks/RP.tga", 22)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/GG.tga", 30)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/P1.tga", 31)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/P2.tga", 32)) != 1)
		std::cout << "ERROR " << err;
	
	if((err = TGA::loadTGA((char*)"src/sprites/DRAW.tga", 33)) != 1)
		std::cout << "ERROR " << err;
	
	
	if(PlayerPrefs::GetBool("First run", true)) {
		showControls = true;
		PlayerPrefs::SetBool("First run", false);
	}
}

/// Function to update scene.
void MainMenu::Update() {
	
}

/// Method called whenever window has been resized
void MainMenu::OnWindowResized() {
	float w = (float)glutGet(GLUT_WINDOW_WIDTH);
	float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
	float menuTop = h*2/3;
	float buttonHeight = h*1/10;
	float buttonWidth = buttonHeight * 4;
	
	// main menu
	background = Sprite(Vector2(0, h), Vector2(w, h), MAINMENU_BACKGROUND);
	spButton = Sprite(Vector2(10, menuTop), Vector2(buttonWidth, buttonHeight), MAINMENU_1PLAYER_NORMAL);
	menuTop -= buttonHeight;
	mpButton = Sprite(Vector2(10, menuTop), Vector2(buttonWidth, buttonHeight), MAINMENU_2PLAYERS_NORMAL);
	menuTop -= buttonHeight;
	controlsButton = Sprite(Vector2(10, menuTop), Vector2(buttonWidth, buttonHeight), MAINMENU_CONTROLS);
	menuTop -= buttonHeight;
	exitButton = Sprite(Vector2(10, menuTop), Vector2(buttonWidth, buttonHeight), MAINMENU_EXIT);
	menuTop += 4*buttonHeight;
	controlsBox = Sprite(Vector2(w-(h/3*2),h*5/6), Vector2(h/3*2,h/3*2), MAINMENU_CONTROLS_BOX);
	controlsBox.SetColor(0,0,0.8f);
	
	// preview
	arrowPrev = Sprite(Vector2(10, h/2 + buttonHeight/2), Vector2(buttonWidth/2, buttonHeight/2), LEVELSELECT_BACK);
	arrowNext = Sprite(Vector2(w - 10 - buttonWidth/2, h/2 + buttonHeight/2), Vector2(buttonWidth/2, buttonHeight/2), LEVELSELECT_NEXT);
	LoadMapPreview();
}

/// Method called when key is pressed
void MainMenu::KeyPress(unsigned char key) {
	if(key == 13) {
		// go to level select menu
		switch(menuPosition) {
			case MENU_EXIT:
				exit = true;
				break;
			case MENU_CONTROLS:
				showControls = !showControls;
				break;
			default:
				if(!showMainMenu && mapLoader.GetMapList(singlePlayer).size() > previewMapIndex) {
					// load level
					Level::levelName = mapLoader.LoadMap(mapLoader.GetMapList(singlePlayer)[previewMapIndex]).GetName();
					Map m = mapLoader.LoadMap(Level::levelName);
					SwitchScene(SCENE_LEVEL);
				}
			
				showMainMenu = false;
				LoadMapPreview();
				break;
		}
	}
}

/// Method called when special key is pressed.
void MainMenu::SpecialKeyPress(int key) {
	if(showMainMenu) {	
		if(key == GLUT_KEY_DOWN)
			menuPosition++;
		else if(key == GLUT_KEY_UP)
			menuPosition--;
		
		// make sure we arent out of range in our menu options
		if(menuPosition < 1) menuPosition = 1;
		if(menuPosition > 4) menuPosition = 4;
		
		singlePlayer = (menuPosition == 1 ? true : false);
	}
	else {
		if(key == GLUT_KEY_RIGHT) {
			if(mapLoader.GetMapList(singlePlayer).size() > previewMapIndex+1) {
				previewMapIndex++;
				LoadMapPreview();
			}			
		}
		else if(key == GLUT_KEY_LEFT)
			if(previewMapIndex > 0) {
				previewMapIndex--;
				LoadMapPreview();
			}
			else {
				mapPreview.clear();
				showMainMenu =  true;
			}
	}
}

void MainMenu::LoadMapPreview() {
	// clear map
	mapPreview.clear();
	
	float w = (float)glutGet(GLUT_WINDOW_WIDTH);
	float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
	Vector2 boundaries(w/2, h/2);
	
	// return if there are no maps with specified index
	if(mapLoader.GetMapList(singlePlayer).size() <= previewMapIndex)
		return;
	
	// get map
	Map map;
	map = mapLoader.LoadMap(mapLoader.GetMapList(singlePlayer)[previewMapIndex]);
	
	float xCount = map.GetX();
	float yCount = map.GetY();
	float xOffset = 0;
	float yOffset = 0;
	
	// calculate block size and offsets
	float blockSize = boundaries.Y() / yCount;
	if(blockSize * xCount <= boundaries.X()) {
		xOffset = (boundaries.X() - (blockSize * xCount)) / 2;
	}
	else {
		blockSize = boundaries.X() / xCount;
		yOffset = (boundaries.Y() - (blockSize * yCount)) / 2;
	}
	
	// clear mapPreview
	mapPreview.clear();
	
	// fill mapPreview
	int c = 0;
	for(int i = 0; i < map.GetY(); i++) {
		for(int j = 0; j < map.GetX(); j++) {
			int texture = PREVIEW_BLOCK_BACKGROUND;
			bool drawBackground = false;
			// switch to determine texture, block type accessed by map[c++]
			int b = map[c++];
			switch(b) {
				case BLOCK_SPACE:
					texture = PREVIEW_BLOCK_BACKGROUND;
					break;
				case BLOCK_SOLID:
					texture = PREVIEW_BLOCK_SOLID;
					break;
				case BLOCK_EXPLODABLE:
					texture = PREVIEW_BLOCK_EXPLODABLE;
					break;
				case BLOCK_PLAYER:
					texture = PREVIEW_BLOCK_BOMBERMAN;
					drawBackground = true;
					break;
				default:
					if(b >= 0 && b <= 9)
						texture = PREVIEW_BLOCK_CREEP;
					drawBackground = true;
					break;
			}
			
			if(drawBackground)
				mapPreview.push_back(
					Sprite(
						Vector2(xOffset+(boundaries.X()/2)+j*blockSize, h-(boundaries.Y()/2)-i*blockSize-yOffset),
						Vector2(blockSize, blockSize),
						PREVIEW_BLOCK_BACKGROUND
					)
				);
			
			mapPreview.push_back(
				Sprite(
					Vector2(xOffset+(boundaries.X()/2)+j*blockSize, h-(boundaries.Y()/2)-i*blockSize-yOffset),
					Vector2(blockSize, blockSize),
					texture
				)
			);
		}
	}
}