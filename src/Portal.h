///
/// Portal class renders a portal that ends the game in single player maps.
///

#ifndef Portal_h
#define Portal_h

#include "Sprite.h"

class Portal : public Sprite {
private:
	bool unlocked;
public:
	Portal();
	bool GetUnlocked() const { return unlocked; }					///< Returns true if portal is open.
	void SetUnlocked(bool unlocked) { this->unlocked = unlocked; }		///< Sets unlocked parameter of portal.
};

#endif // Portal_h