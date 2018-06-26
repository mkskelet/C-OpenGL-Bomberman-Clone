///
/// Portal class renders a portal that ends the game in single player maps.
///

#include "Portal.h"

Portal::Portal() : Sprite(Vector2(0,0), Vector2(0,0), 18) {
	unlocked = false;
}