///
/// Character controlled by AI.
///

#include "Enemy.h"


Enemy::Enemy() : loaded(false) {
	
}

Enemy::Enemy(CharacterType characterType, float speed, float collisionDetectionRange, Vector2 position, Vector2 size, int textureID, int pivot) :
	Character(characterType, speed, collisionDetectionRange, position, size, textureID, pivot) {
	dir = UpD;
	move = false;
	loaded=false;
}

Enemy::~Enemy() {
}

void Enemy::Start() {
	loaded = true;
}

void Enemy::Update() {	
	if(!loaded) return;
	
	// translate
	Vector2 movement(0,0);
	
	if(dir == UpD)
		movement = Vector2(0,1);
	else if(dir == DownD)
		movement = Vector2(0,-1);
	else if(dir == LeftD)
		movement = Vector2(-1,0);
	else if(dir == RightD)
		movement = Vector2(1,0);
	
	Translate(movement);
	
	if(colliding) {
		switch(dir) {
			case UpD:
				dir = RightD;
				break;
			case RightD:
				dir = DownD;
				break;
			case DownD:
				dir = LeftD;
				break;
			default:
				dir = UpD;
				break;
		}
	}
	
	// check for colision with explosions
	Character::Update();
}