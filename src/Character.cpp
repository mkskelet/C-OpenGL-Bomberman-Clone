///
/// Base class for characters in game. Players and computer controlled enemies.
///

#include "Character.h"

// TEMP
#include <iostream>
using namespace std;

Character::Character() : characterType(PlayerCharacter), speed(5.0f), collisionDetectionRange(5), colliding(false), alive(true) {
	
}

Character::Character(CharacterType characterType, float speed, float collisionDetectionRange) {
	this->characterType = characterType;
	this->speed = speed;
	this->collisionDetectionRange = collisionDetectionRange;
	colliding = false;
	alive = true;
}

Character::Character(CharacterType characterType, float speed, float collisionDetectionRange, Vector2 position, Vector2 size, int textureID, int pivot) :
				Sprite(position, size, textureID, pivot) {
	this->characterType = characterType;
	this->speed = speed;
	this->collisionDetectionRange = collisionDetectionRange;
	colliding = false;
	alive = true;
}

void Character::Translate(Vector2 translation) {
	Vector2 newPos = GetPosition() + (translation * speed);
	
	for(int i = 0; i < colliders.size(); i++) {
		bool notColliding = newPos.X() > colliders[i].GetPosition().X() + colliders[i].GetScale().X() ||
						colliders[i].GetPosition().X() > newPos.X() + GetScale().X() ||
						newPos.Y() - GetScale().Y() > colliders[i].GetPosition().Y() ||
						colliders[i].GetPosition().Y() - colliders[i].GetScale().Y() > newPos.Y();
		if(!notColliding) {
			//cout << "COLLISION\n";
			newPos = GetPosition();
			colliding = true;
			break;
		}
		else colliding = false;
	}
	
	SetPosition(newPos);
}

void Character::UpdateGeneralColisions(std::vector<Sprite> colliders) {
	this->colliders = colliders;
}

void Character::UpdateExplosionColisions(std::vector<Explosion> colliders) {
	exColliders = colliders;
}

void Character::Render() {
	if(alive)
		Sprite::Render();
}

void Character::Start() {
}

void Character::Update() {
	for(int i = 0; i < exColliders.size(); i++) {
		bool notColliding = GetPosition().X() > exColliders[i].GetPosition().X() + exColliders[i].GetScale().X() ||
						exColliders[i].GetPosition().X() > GetPosition().X() + GetScale().X() ||
						GetPosition().Y() - GetScale().Y() > exColliders[i].GetPosition().Y() ||
						exColliders[i].GetPosition().Y() - exColliders[i].GetScale().Y() > GetPosition().Y();
		if(!notColliding) {
			alive = false;
		}
	}
}