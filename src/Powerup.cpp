///
/// Renders a powerup, of some type.
///

#include "Powerup.h"

Powerup::Powerup() : Sprite(Vector2(0,0), Vector2(0,0), 19) {
	pt = BombP;
}

void Powerup::SetPowerupType(PowerupType pt) {
	this->pt = pt;
	Sprite::SetTextureID(pt);
}