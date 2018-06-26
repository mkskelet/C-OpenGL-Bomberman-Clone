///
/// Character controlled by player.
///

#include "Player.h"

// TEMP
#include <iostream>
using namespace std;

Player::Player() : loaded(false) {
	keyStates = new bool[256];
	for(int i=0; i<256; i++)
		keyStates[i] = false;
	
	specialKeyStates = new bool[256];
	for(int i=0; i<256; i++)
		specialKeyStates[i] = false;
	
	bombCount = 1;
	bombRange = 3;
	remote = false;
	
	inPortal = false;
}

Player::Player(int playerNumber, CharacterType characterType, float speed, float collisionDetectionRange, Vector2 position, Vector2 size, int textureID, int pivot) :
	Character(characterType, speed, collisionDetectionRange, position, size, textureID, pivot) {
	this->controls = ((playerNumber == PLAYER_ONE || playerNumber == PLAYER_TWO) ? playerNumber : PLAYER_ONE);
	dir = UpD;
	move = false;
	loaded=false;
	keyStates = new bool[256];
	for(int i=0; i<256; i++)
		keyStates[i] = false;
		
	specialKeyStates = new bool[256];
	for(int i=0; i<256; i++)
		specialKeyStates[i] = false;
		
	bombCount = 1;
	bombRange = 3;
	remote = false;
		
	inPortal = false;
}

Player::~Player() {
}

void Player::Start() {
	loaded = true;
}

void Player::Update() {	
	if(!loaded) return;
	
	// translate
	Vector2 movement(0,0);
	
	if(controls == PLAYER_TWO) {
		if(keyStates['e'])
			movement = Vector2(0,1);
		else if(keyStates['d'])
			movement = Vector2(0,-1);
		else if(keyStates['s'])
			movement = Vector2(-1,0);
		else if(keyStates['f'])
			movement = Vector2(1,0);
	}
	
	if(controls == PLAYER_ONE) {
		if(specialKeyStates[GLUT_KEY_UP])
			movement = Vector2(0,1);
		else if(specialKeyStates[GLUT_KEY_DOWN])
			movement = Vector2(0,-1);
		else if(specialKeyStates[GLUT_KEY_LEFT])
			movement = Vector2(-1,0);
		else if(specialKeyStates[GLUT_KEY_RIGHT])
			movement = Vector2(1,0);
	}
	
	Translate(movement);
	
	// check for colision with enemy
	for(int i=0; i<characters.size(); i++) {
		bool notColliding = GetPosition().X() > characters[i].GetPosition().X() + characters[i].GetScale().X() ||
						characters[i].GetPosition().X() > GetPosition().X() + GetScale().X() ||
						GetPosition().Y() - GetScale().Y() > characters[i].GetPosition().Y() ||
						characters[i].GetPosition().Y() - characters[i].GetScale().Y() > GetPosition().Y();
		if(!notColliding) {
			// DED
			alive = false;
			break;
		}
	}
	
	// check for colision with explosions
	Character::Update();
}

void Player::Render() {
	if(alive)
		Character::Render();
}

void Player::KeyPress(unsigned char key) {
	if(!loaded) return;
	
	keyStates[key] = true;
}

void Player::SpecialKeyPress(int key) {
	if(!loaded) return;
	
	specialKeyStates[key] = true;
}

void Player::SpecialKeyRelease(int key) {
	if(!loaded) return;
	
	specialKeyStates[key] = false;
}

void Player::KeyRelease(unsigned char key) {
	if(!loaded) return;
	
	keyStates[key] = false;
}

void Player::UpdateCharacterColisions(std::vector<Enemy> colliders) {
	this->characters = colliders;
}

/*void Player::UpdatePowerupColisions(std::vector<Powerups> colliders) {
	powerups = colliders;
}*/

void Player::UpdatePortalColision(Portal collider) {
	portalCollider = collider;
}

bool Player::IsInPortal() {
	// check for colision with portal
	bool notColliding = GetPosition().X() > portalCollider.GetPosition().X() + portalCollider.GetScale().X() ||
					portalCollider.GetPosition().X() > GetPosition().X() + GetScale().X() ||
					GetPosition().Y() - GetScale().Y() > portalCollider.GetPosition().Y() ||
					portalCollider.GetPosition().Y() - portalCollider.GetScale().Y() > GetPosition().Y();
	if(!notColliding) {
		return true;
	}
	
	return false;
}

bool Player::PickPowerup(Powerup powerup) {
	// check for colision with powerups
	bool notColliding = GetPosition().X() > powerup.GetPosition().X() + powerup.GetScale().X() ||
					powerup.GetPosition().X() > GetPosition().X() + GetScale().X() ||
					GetPosition().Y() - GetScale().Y() > powerup.GetPosition().Y() ||
					powerup.GetPosition().Y() - powerup.GetScale().Y() > GetPosition().Y();
	if(!notColliding) {
		// pick powerup
		switch(powerup.GetPowerupType()) {
			case BombP:
				bombCount++;
				break;
			case FlameP:
				bombRange++;
				break;
			case SpeedP:
				SetSpeed(GetSpeed() * 1.1f);
				break;
			case RemoteP:
				remote = true;
				break;
		}
		return true;
	}
	return false;
}