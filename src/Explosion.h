///
/// Explosion class renders a fire
///

#ifndef Explosion_h
#define Explosion_h

#include "Sprite.h"
#include "GameTime.h"

#define TIME_TO_DISAPPEAR 1000

class Explosion : public Sprite {
private:
	long timeToDisappear;
public:
	Explosion();
	bool GetDisappear() const;
};

#endif