///
/// Character controlled by AI.
///

#ifndef Enemy_h
#define Enemy_h

#include "Character.h"

class Enemy : public Character {
private:
	bool loaded;
	bool move;
	int controls;
	Direction dir;
public:
	Enemy();
	~Enemy();
	Enemy(CharacterType characterType, float speed, float collisionDetectionRange, Vector2 position, Vector2 size, int textureID, int pivot);
	void Start();
	void Update();
};

#endif // Enemy_h