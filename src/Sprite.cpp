///
/// Class takes care of rendering and moving 2D textured quads AKA Sprites.
///

#include "Sprite.h"
#include "tga.h"
#include <iostream>

/// Constructor to create sprite with specified position, scale and textureID, default pivot PIVOT_CENTER is used.
Sprite::Sprite(Vector2 position, Vector2 scale, int textureID) {
	this->position = position;
	this->scale = scale;
	this->textureID = textureID;
	spritePivot = PIVOT_TOPLEFT;
	colorR = 1.0f;
	colorG = 1.0f;
	colorB = 1.0f;
	colorA = 1.0f;
}

/// Constructor to create sprite with specified position, scale and textureID and pivot.
Sprite::Sprite(Vector2 position, Vector2 scale, int textureID, int spritePivot) {
	this->position = position;
	this->scale = scale;
	this->textureID = textureID;
	SetSpritePivot(spritePivot);
	colorR = 1.0f;
	colorG = 1.0f;
	colorB = 1.0f;
	colorA = 1.0f;
}

void Sprite::Render() {
	// set color
	glColor3f(colorR,colorG,colorB);
	
	// prepare for drawing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	
	// draw textured quad
	if(spritePivot == PIVOT_TOPLEFT) {
		glTexCoord2f(0,0); glVertex2d(position.X(), position.Y() - scale.Y());
		glTexCoord2f(1,0); glVertex2d(position.X() + scale.X(), position.Y() - scale.Y());
		glTexCoord2f(1,1); glVertex2d(position.X() + scale.X(), position.Y());
		glTexCoord2f(0,1); glVertex2d(position.X(), position.Y());
	}
	
	// end drawing and reset color
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glColor3f(1.0f,1.0f,1.0f);
}

void Sprite::Start() {
	
}

void Sprite::Update() {
	
}