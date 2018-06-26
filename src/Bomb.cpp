///
/// Bomb class renders a bomb
///

#include "Bomb.h"

Bomb::Bomb() : Sprite(Vector2(0,0), Vector2(0,0), 16) {
	range = 3;
	explosionTime = GameTime::time + TIME_TO_EXPLODE;
	remoteFireEnabled = false;
	fire = false;
}

bool Bomb::GetFire() const {
	if(remoteFireEnabled && fire)
		return true;
	else if(!remoteFireEnabled && explosionTime <= GameTime::GetTime())
		return true;
	else return false;
}