///
/// Level scene class. Renders map, counts score.
///

#ifndef Level_h
#define Level_h

#include "Scene.h"
#include "MapLoader.h"
#include "Sprite.h"
#include "Player.h"
#include "Bomb.h"
#include "Explosion.h"
#include "Enemy.h"
#include "Portal.h"
#include "GameTime.h"
#include "Powerup.h"
#include <vector>
#include <string>

class Level : public Scene {
private:
	std::vector<int> takenNodes;			///< contains indexes of all blocks that have powerup or portal underneath
	std::vector<Powerup> powerups;		///< powerup pickups for player
	std::vector<Vector2> exploded;		///< blocks that exploded
	std::vector<Explosion> explosions;		///< fire sprites
	std::vector<Sprite> backgroundBlocks;	///< background blocks	
	std::vector<Sprite> solidBlocks;		///< static blocks
	std::vector<Sprite> otherBlocks;		///< explodable blocks, powerups, portal	
	std::vector<Bomb> bombs1;			///< bombs belonging to player1
	std::vector<Bomb> bombs2;			///< bombs belonging to player2
	std::vector<Enemy> ai;				///< finally render characters
	long levelTime;					///< time when level ends
	int timeRemaining;
	Map map;
	Sprite topBar;
	Sprite endGame;
	Sprite endGameP1;
	Sprite endGameP2;
	Sprite endGameD;
	Player p1;
	Player p2;
	bool loaded;						///< Indicates if map was already loaded.
	Portal portal;						///< Portal that ends game.
	bool playerWon;
public:
	static std::string levelName;
	Level();
	void LoadLevel();				///< Call to load map, prepare level for rendering, reset time
	void Render();					///< Renders level and everything in it.	
	void Update();
	void Start();
	void OnWindowResized();
	void KeyPress(unsigned char key);
	void SpecialKeyPress(int key);
	void SpecialKeyRelease(int key);
	void KeyRelease(unsigned char key);
};

#endif