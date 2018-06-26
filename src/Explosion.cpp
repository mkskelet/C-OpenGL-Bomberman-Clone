///
/// Explosion class renders a fire
///

#include "Explosion.h"

Explosion::Explosion() : Sprite(Vector2(0,0), Vector2(0,0), 17) {
	timeToDisappear = GameTime::time + TIME_TO_DISAPPEAR;
}

bool Explosion::GetDisappear() const {
	if(timeToDisappear <= GameTime::time)
		return true;
	
	else return false;
}