///
/// Base class for characters in game. Players and computer controlled enemies.
///

#ifndef Character_h
#define Character_h

#include "Sprite.h"
#include <vector>
#include "Explosion.h"

enum CharacterType { PlayerCharacter = 0, EnemyCharacter = 1 };
enum ColisionFlags { Top = 0, Bottom = 1, Left = 2, Right = 3 };
enum Direction { UpD = 0, DownD = 1, LeftD = 2, RightD = 3 };

class Character : public Sprite {
private:
	std::vector<Sprite> colliders;			///< Colliders to test colision against.
	std::vector<Explosion> exColliders;		///< Explosions to test colision against.
	CharacterType characterType;			///< Character type.
	float speed;						///< Character movement speed.
	float collisionDetectionRange;			///< Range around characters sprite in which character detect colision.
protected:
	bool colliding;						///< Bool to detect if character is colliding or not
	bool alive;
public:
	Character();
	Character(CharacterType characterType, float speed, float collisionDetectionRange);
	Character(CharacterType characterType, float speed, float collisionDetectionRange, Vector2 position, Vector2 size, int textureID, int pivot);
	void Translate(Vector2 translation);								///< Moves the character in direction specified by translation.
	void UpdateGeneralColisions(std::vector<Sprite> colliders);			///< Updates the colision flags. Necessary for movement.
	void UpdateExplosionColisions(std::vector<Explosion> colliders);		///< Check if character collides with fire.
	virtual void Render();
	virtual void Update();
	virtual void Start();
	void SetSpeed(float speed) { this->speed = speed; }
	float GetSpeed() const { return speed; }
	bool IsAlive() { return alive; }
};

#endif // Character_h