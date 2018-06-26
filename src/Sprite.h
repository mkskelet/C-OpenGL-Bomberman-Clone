///
/// Class takes care of rendering and moving 2D textured quads AKA Sprites.
///

#ifndef Sprite_h
#define Sprite_h

#include <GL/glut.h>
#include "Vector2.h"

// sprite pivot
#define PIVOT_TOPLEFT 0
#define PIVOT_CENTER 1

class Sprite {
private:
	Vector2 position;		///< Position of sprite on the screen.
	Vector2 scale;			///< Size of sprite in pixels.
	int textureID;			///< ID of texture to fill sprite with (0 for no texture).
	int spritePivot;			///< Pivot mode. See the definition.
	float colorR;
	float colorG;
	float colorB;
	float colorA;
public:
	Sprite() : textureID(-1), spritePivot(PIVOT_TOPLEFT), colorR(1.0f), colorG(1.0f), colorB(1.0f), colorA(1.0f) {};
	Sprite(Vector2 position, Vector2 scale, int textureID);
	Sprite(Vector2 position, Vector2 scale, int textureID, int spritePivot);
	void SetPosition(Vector2 position) { this->position = position; }
	void SetScale(Vector2 scale) { this->scale = scale; }
	void SetTextureID(int id) { textureID = id; }
	void SetSpritePivot(int pivot) { spritePivot = (pivot < 0 ? PIVOT_TOPLEFT : (pivot > 1 ? PIVOT_CENTER : pivot)); }
	void SetColor(float r, float g, float b) { colorR = r; colorG = g; colorB = b; colorA = 1.0f; }
	void SetColor(float r, float g, float b, float a) { colorR = r; colorG = g; colorB = b; colorA = a; }
	Vector2 GetPosition() const { return position; }
	Vector2 GetScale() const {return scale; }
	int GetTextureID() const { return textureID; }
	void Render();			///< Method used to render a sprite.
	void Start();			///< Method called after constructing the object.
	void Update();			///< Method called every frame.
};

#endif // Sprite_h