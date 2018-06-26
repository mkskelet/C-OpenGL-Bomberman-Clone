///
/// Level scene class. Renders map, counts score.
///

#include "Level.h"
#include <cmath>
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include "PlayerPrefs.h"

// define blocks
#define LEVEL_BLOCK_BACKGROUND 11
#define LEVEL_BLOCK_SOLID 12
#define LEVEL_BLOCK_EXPLODABLE 13

using namespace std;

string Level::levelName = "";

Level::Level() : Scene(SCENE_LEVEL), loaded(false), playerWon(false) {
	timeRemaining = 1000000;
}

void Level::LoadLevel() {
	MapLoader ml;
	map = ml.LoadMap(levelName);
}

void Level::Render() {
	// render top bar
	topBar.Render();
	
	// render time remaining
	{
		stringstream ss;
		string s;
		float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
		glColor3f(0.7f,0.0f,1.0f);
		glRasterPos2f(0, h-20);
		ss << (timeRemaining <= 0 ? 0 : timeRemaining);
		s = ss.str();
		const char* arr1 = s.c_str();
		int len = (int)strlen(arr1);
		for(int i = 0; i < len; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, arr1[i]);
		}
		glColor3f(1.0f,1.0f,1.0f);
	}
	
	// render high score
	if(map.GetSinglePlayer()) {
		glColor3f(0.7f,0.0f,1.0f);
		float w = (float)glutGet(GLUT_WINDOW_WIDTH);
		float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
		glRasterPos2f(w-100, h-20);
		{
			stringstream ss;
			string s;
			ss << "HS: " << PlayerPrefs::GetInt(map.GetName(), 0);
			s = ss.str();
			const char* arr1 = s.c_str();
			int len = (int)strlen(arr1);
			for(int i = 0; i < len; i++) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, arr1[i]);
			}
		}
		glColor3f(1.0f,1.0f,1.0f);
	}
	
	// render grass
	if(backgroundBlocks.size() >0) {
		for(unsigned int i=0; i<backgroundBlocks.size(); i++) {
			backgroundBlocks[i].Render();
		}
	}
	
	// render portal
	portal.Render();
	
	// render powerups
	for(unsigned int i=0; i<powerups.size(); i++) {
		powerups[i].Render();
	}
	
	// render static blocks
	if(solidBlocks.size() >0) {
		for(unsigned int i=0; i<solidBlocks.size(); i++) {
			solidBlocks[i].Render();
		}
	}
	
	// render other blocks
	if(otherBlocks.size() >0) {
		for(unsigned int i=0; i<otherBlocks.size(); i++) {
			otherBlocks[i].Render();
		}
	}
	
	// render bombs
	if(bombs1.size() >0) {
		for(unsigned int i=0; i<bombs1.size(); i++) {
			bombs1[i].Render();
		}
	}
	if(bombs2.size() >0) {
		for(unsigned int i=0; i<bombs2.size(); i++) {
			bombs2[i].Render();
		}
	}
	
	// render characters
	if(ai.size() >0) {
		for(unsigned int i=0; i<ai.size(); i++) {
			ai[i].Render();
		}
	}
	
	// render player
	p2.Render();
	//if(!map.GetSinglePlayer())
		p1.Render();
	
	// render explosions
	if(explosions.size() >0) {
		for(unsigned int i=0; i<explosions.size(); i++) {
			explosions[i].Render();
		}
	}
	
	// render endgame
	if((map.GetSinglePlayer() && !p2.IsAlive()) || timeRemaining <= 0) {
		// fail screen
		endGame.Render();
	}
	else if(map.GetSinglePlayer() && p2.IsAlive() && playerWon) {
		endGameP1.Render();
	}
	else if(!map.GetSinglePlayer()) {
		if(!p1.IsAlive() && !p2.IsAlive())
			endGameD.Render();
		else if(!p2.IsAlive())
			endGameP1.Render();
		else if(!p1.IsAlive())
			endGameP2.Render();
		else if(timeRemaining <= 0)
			endGameD.Render();
	}
}

void Level::Update() {
	// update time
	timeRemaining = levelTime - GameTime::time;
	
	// delete explosion if necessary
	if(explosions.size() >0) {
		for(int i=explosions.size()-1; i>=0; i--) {
			if(explosions[i].GetDisappear())
				explosions.erase(explosions.begin() + i);
		}
	}
	
	// update enemies
	if(ai.size() >0) {
		for(int i=ai.size()-1; i>=0; i--) {
			ai[i].UpdateGeneralColisions(solidBlocks);
			ai[i].UpdateExplosionColisions(explosions);
			ai[i].Update();
			
			if(!ai[i].IsAlive())
				ai.erase(ai.begin() + i);
		}
	}
	
	// update portal
	if(ai.size() == 0)
		portal.SetUnlocked(true);
	
	p2.UpdateGeneralColisions(solidBlocks);
	p2.UpdateCharacterColisions(ai);
	p2.UpdateExplosionColisions(explosions);
	p2.UpdatePortalColision(portal);
	p2.Update();
	
	p1.UpdateGeneralColisions(solidBlocks);
	p1.UpdateCharacterColisions(ai);
	p1.UpdateExplosionColisions(explosions);
	p1.UpdatePortalColision(portal);
	p1.Update();
	
	// check portal colision
	if(map.GetSinglePlayer() && p2.IsInPortal() && portal.GetUnlocked()) {
		// saev score
		//cout << "SAVING SCORE " << timeRemaining << "\n";
		if(!playerWon && map.GetSinglePlayer())
			PlayerPrefs::SetInt(map.GetName(), timeRemaining);
		playerWon = true;
	}
	if(!map.GetSinglePlayer() && map.GetSinglePlayer() && p1.IsInPortal() && portal.GetUnlocked()) {
		playerWon = true;
	}
	
	// check powerups colision
	if(powerups.size() > 0) {
		for(int i=powerups.size()-1; i>=0; i--) {
			if(p2.PickPowerup(powerups[i])) {
				powerups.erase(powerups.begin() + i);
			}
		}
		if(!map.GetSinglePlayer())
			for(int i=powerups.size()-1; i>=0; i--) {
				if(p1.PickPowerup(powerups[i])) {
					powerups.erase(powerups.begin() + i);
				}
			}
	}
	
	// explode bombs
	if(bombs1.size() >0) {
		for(unsigned int i=0; i<bombs1.size(); i++) {
			if(bombs1[i].GetFire()) {
				// find closest position on the grid
				Vector2 closest(-1,-1);
				if(backgroundBlocks.size() >0) {
					for(unsigned int k=0; k<backgroundBlocks.size(); k++) {
						float c, j;
						c = sqrt(pow(fabs(bombs1[i].GetPosition().X()-closest.X()), 2) + pow(fabs(bombs1[i].GetPosition().Y()-closest.Y()), 2));
						j = sqrt(pow(fabs(bombs1[i].GetPosition().X()-backgroundBlocks[k].GetPosition().X()), 2) + pow(fabs(bombs1[i].GetPosition().Y()-backgroundBlocks[k].GetPosition().Y()), 2));
						
						if(j < c) closest = backgroundBlocks[k].GetPosition();
					}
				}
				
				std::vector<int> indexToDestroy;
				bool skip = false;
				
				// EXPLODE RIGHT SIDE
				for(int j = 0; j < bombs1[i].GetRange(); j++) {
					Vector2 next(closest.X() + j * backgroundBlocks[0].GetScale().X(), closest.Y());
					
					// create explosion effect
					if(!skip) {
						Explosion b;
						b.SetPosition(next);
						b.SetScale(p2.GetScale());
						explosions.push_back(b);
					}
					
					if(solidBlocks.size() >0 && !skip) {
						for(unsigned int k=0; k<solidBlocks.size(); k++) {
							if(solidBlocks[k].GetPosition().X() <= next.X()+2 && solidBlocks[k].GetPosition().X() >= next.X()-2
								&& solidBlocks[k].GetPosition().Y() <= next.Y()+2 && solidBlocks[k].GetPosition().Y() >= next.Y()-2) {
								if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
									indexToDestroy.push_back(k);
									skip = true;
									break;
								}
								else if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_SOLID) {
									explosions.erase(explosions.begin() + explosions.size() - 1);
									skip = true;
									break;
								}
							}
						}
					}
				}
				skip = false;
				
				// EXPLODE LEFT SIDE
				for(int j = 1; j < bombs1[i].GetRange(); j++) {
					Vector2 next(closest.X() - j * backgroundBlocks[0].GetScale().X(), closest.Y());
					
					// create explosion effect
					if(!skip) {
						Explosion b;
						b.SetPosition(next);
						b.SetScale(p2.GetScale());
						explosions.push_back(b);
					}
					
					if(solidBlocks.size() >0 && !skip) {
						for(unsigned int k=0; k<solidBlocks.size(); k++) {
							if(solidBlocks[k].GetPosition().X() <= next.X()+2 && solidBlocks[k].GetPosition().X() >= next.X()-2
								&& solidBlocks[k].GetPosition().Y() <= next.Y()+2 && solidBlocks[k].GetPosition().Y() >= next.Y()-2) {
								if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
									indexToDestroy.push_back(k);
									skip = true;
									break;
								}
								else if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_SOLID) {
									explosions.erase(explosions.begin() + explosions.size() - 1);
									skip = true;
									break;
								}
							}
						}
					}
				}
				skip = false;
				
				// EXPLODE TOP SIDE
				for(int j = 1; j < bombs1[i].GetRange(); j++) {
					Vector2 next(closest.X(), closest.Y() + j * backgroundBlocks[0].GetScale().Y());
					
					// create explosion effect
					if(!skip) {
						Explosion b;
						b.SetPosition(next);
						b.SetScale(p2.GetScale());
						explosions.push_back(b);
					}
					
					if(solidBlocks.size() >0 && !skip) {
						for(unsigned int k=0; k<solidBlocks.size(); k++) {
							if(solidBlocks[k].GetPosition().X() <= next.X()+2 && solidBlocks[k].GetPosition().X() >= next.X()-2
								&& solidBlocks[k].GetPosition().Y() <= next.Y()+2 && solidBlocks[k].GetPosition().Y() >= next.Y()-2) {
								if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
									indexToDestroy.push_back(k);
									skip = true;
									break;
								}
								else if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_SOLID) {
									explosions.erase(explosions.begin() + explosions.size() - 1);
									skip = true;
									break;
								}
							}
						}
					}
				}
				skip = false;
				
				// EXPLODE BOTTOM SIDE
				for(int j = 1; j < bombs1[i].GetRange(); j++) {
					Vector2 next(closest.X(), closest.Y() - j * backgroundBlocks[0].GetScale().Y());
					
					// create explosion effect
					if(!skip) {
						Explosion b;
						b.SetPosition(next);
						b.SetScale(p2.GetScale());
						explosions.push_back(b);
					}
					
					if(solidBlocks.size() >0 && !skip) {
						for(unsigned int k=0; k<solidBlocks.size(); k++) {
							if(solidBlocks[k].GetPosition().X() <= next.X()+2 && solidBlocks[k].GetPosition().X() >= next.X()-2
								&& solidBlocks[k].GetPosition().Y() <= next.Y()+2 && solidBlocks[k].GetPosition().Y() >= next.Y()-2) {
								if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
									indexToDestroy.push_back(k);
									skip = true;
									break;
								}
								else if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_SOLID) {
									explosions.erase(explosions.begin() + explosions.size() - 1);
									skip = true;
									break;
								}
							}
						}
					}
				}
				
				std::sort(indexToDestroy.begin(), indexToDestroy.end(), std::greater<int>());
				for(unsigned int j = 0; j < indexToDestroy.size(); j++) {
					// vymazanim posuvam indexy, treba sortnut vektor a mazat od najvacsieho indexu
					// najprv vyrenderujeme exploziu
					
					// potom vymazeme objekty
					solidBlocks.erase(solidBlocks.begin() + indexToDestroy[j]);
				}
				indexToDestroy.clear();
				
				// explode				
				bombs1.erase(bombs1.begin() + i);
			}
		}
	}
	
	// explode bombs2
	if(bombs2.size() >0) {
		for(unsigned int i=0; i<bombs2.size(); i++) {
			if(bombs2[i].GetFire()) {
				// find closest position on the grid
				Vector2 closest(-1,-1);
				if(backgroundBlocks.size() >0) {
					for(unsigned int k=0; k<backgroundBlocks.size(); k++) {
						float c, j;
						c = sqrt(pow(fabs(bombs2[i].GetPosition().X()-closest.X()), 2) + pow(fabs(bombs2[i].GetPosition().Y()-closest.Y()), 2));
						j = sqrt(pow(fabs(bombs2[i].GetPosition().X()-backgroundBlocks[k].GetPosition().X()), 2) + pow(fabs(bombs2[i].GetPosition().Y()-backgroundBlocks[k].GetPosition().Y()), 2));
						
						if(j < c) closest = backgroundBlocks[k].GetPosition();
					}
				}
				
				std::vector<int> indexToDestroy;
				bool skip = false;
				
				// EXPLODE RIGHT SIDE
				for(int j = 0; j < bombs2[i].GetRange(); j++) {
					Vector2 next(closest.X() + j * backgroundBlocks[0].GetScale().X(), closest.Y());
					
					// create explosion effect
					if(!skip) {
						Explosion b;
						b.SetPosition(next);
						b.SetScale(p2.GetScale());
						explosions.push_back(b);
					}
					
					if(solidBlocks.size() >0 && !skip) {
						for(unsigned int k=0; k<solidBlocks.size(); k++) {
							if(solidBlocks[k].GetPosition().X() <= next.X()+2 && solidBlocks[k].GetPosition().X() >= next.X()-2
								&& solidBlocks[k].GetPosition().Y() <= next.Y()+2 && solidBlocks[k].GetPosition().Y() >= next.Y()-2) {
								if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
									indexToDestroy.push_back(k);
									skip = true;
									break;
								}
								else if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_SOLID) {
									explosions.erase(explosions.begin() + explosions.size() - 1);
									skip = true;
									break;
								}
							}
						}
					}
				}
				skip = false;
				
				// EXPLODE LEFT SIDE
				for(int j = 1; j < bombs2[i].GetRange(); j++) {
					Vector2 next(closest.X() - j * backgroundBlocks[0].GetScale().X(), closest.Y());
					
					// create explosion effect
					if(!skip) {
						Explosion b;
						b.SetPosition(next);
						b.SetScale(p2.GetScale());
						explosions.push_back(b);
					}
					
					if(solidBlocks.size() >0 && !skip) {
						for(unsigned int k=0; k<solidBlocks.size(); k++) {
							if(solidBlocks[k].GetPosition().X() <= next.X()+2 && solidBlocks[k].GetPosition().X() >= next.X()-2
								&& solidBlocks[k].GetPosition().Y() <= next.Y()+2 && solidBlocks[k].GetPosition().Y() >= next.Y()-2) {
								if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
									indexToDestroy.push_back(k);
									skip = true;
									break;
								}
								else if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_SOLID) {
									explosions.erase(explosions.begin() + explosions.size() - 1);
									skip = true;
									break;
								}
							}
						}
					}
				}
				skip = false;
				
				// EXPLODE TOP SIDE
				for(int j = 1; j < bombs2[i].GetRange(); j++) {
					Vector2 next(closest.X(), closest.Y() + j * backgroundBlocks[0].GetScale().Y());
					
					// create explosion effect
					if(!skip) {
						Explosion b;
						b.SetPosition(next);
						b.SetScale(p2.GetScale());
						explosions.push_back(b);
					}
					
					if(solidBlocks.size() >0 && !skip) {
						for(unsigned int k=0; k<solidBlocks.size(); k++) {
							if(solidBlocks[k].GetPosition().X() <= next.X()+2 && solidBlocks[k].GetPosition().X() >= next.X()-2
								&& solidBlocks[k].GetPosition().Y() <= next.Y()+2 && solidBlocks[k].GetPosition().Y() >= next.Y()-2) {
								if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
									indexToDestroy.push_back(k);
									skip = true;
									break;
								}
								else if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_SOLID) {
									explosions.erase(explosions.begin() + explosions.size() - 1);
									skip = true;
									break;
								}
							}
						}
					}
				}
				skip = false;
				
				// EXPLODE BOTTOM SIDE
				for(int j = 1; j < bombs2[i].GetRange(); j++) {
					Vector2 next(closest.X(), closest.Y() - j * backgroundBlocks[0].GetScale().Y());
					
					// create explosion effect
					if(!skip) {
						Explosion b;
						b.SetPosition(next);
						b.SetScale(p2.GetScale());
						explosions.push_back(b);
					}
					
					if(solidBlocks.size() >0 && !skip) {
						for(unsigned int k=0; k<solidBlocks.size(); k++) {
							if(solidBlocks[k].GetPosition().X() <= next.X()+2 && solidBlocks[k].GetPosition().X() >= next.X()-2
								&& solidBlocks[k].GetPosition().Y() <= next.Y()+2 && solidBlocks[k].GetPosition().Y() >= next.Y()-2) {
								if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
									indexToDestroy.push_back(k);
									skip = true;
									break;
								}
								else if(solidBlocks[k].GetTextureID() == LEVEL_BLOCK_SOLID) {
									explosions.erase(explosions.begin() + explosions.size() - 1);
									skip = true;
									break;
								}
							}
						}
					}
				}
				
				std::sort(indexToDestroy.begin(), indexToDestroy.end(), std::greater<int>());
				for(unsigned int j = 0; j < indexToDestroy.size(); j++) {
					// vymazanim posuvam indexy, treba sortnut vektor a mazat od najvacsieho indexu
					// najprv vyrenderujeme exploziu
					
					// potom vymazeme objekty
					solidBlocks.erase(solidBlocks.begin() + indexToDestroy[j]);
				}
				indexToDestroy.clear();
				
				// explode				
				bombs2.erase(bombs2.begin() + i);
			}
		}
	}
}

void Level::Start() {
	srand(time(NULL));
	
	LoadLevel();
	levelTime = map.GetTimeLimit()*1000 + GameTime::time;
	
	OnWindowResized();
	
	// calculate max index for powerups
	int maxIndex = 0;
	if(solidBlocks.size() >0) {
		for(unsigned int i=0; i<solidBlocks.size(); i++) {
			if(solidBlocks[i].GetTextureID() == LEVEL_BLOCK_EXPLODABLE)
				maxIndex++;
		}
	}
	
	// generate portal
	if(map.GetSinglePlayer()) {
		int portalIndex = rand() % maxIndex;		
		takenNodes.push_back(portalIndex);
		
		int tempIndex = 0;
		if(solidBlocks.size() >0) {
			for(unsigned int i=0; i<solidBlocks.size(); i++) {
				if(solidBlocks[i].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
					if(tempIndex == portalIndex) {
						// set portal position
						portal.SetPosition(solidBlocks[i].GetPosition());
						portal.SetScale(solidBlocks[i].GetScale());
						break;
					}
					
					tempIndex++;
				}
			}
		}
	}
	
	// generate bomb powerups
	for(int i=0; i<map.GetBombPowerups(); i++) {
		bool freee = false;
		do {
			int powerupIndex = rand() % maxIndex;	
			
			if(find(takenNodes.begin(), takenNodes.end(), powerupIndex) != takenNodes.end())
				continue;
			
			takenNodes.push_back(powerupIndex);
			
			int tempIndex = 0;
			for(unsigned int i=0; i<solidBlocks.size(); i++) {
				if(solidBlocks[i].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
					if(tempIndex == powerupIndex) {
						// set powerup position
						Powerup p;
						p.SetPowerupType(BombP);
						p.SetPosition(solidBlocks[i].GetPosition());
						p.SetScale(solidBlocks[i].GetScale());
						
						powerups.push_back(p);
						break;
					}
					
					tempIndex++;
				}
			}
			
			freee = true;
		} while (freee != true);
	}
	
	// generate flame powerups
	for(int i=0; i<map.GetFlamePowerups(); i++) {
		bool freee = false;
		do {
			int powerupIndex = rand() % maxIndex;	
			
			if(find(takenNodes.begin(), takenNodes.end(), powerupIndex) != takenNodes.end())
				continue;
			
			takenNodes.push_back(powerupIndex);
			
			int tempIndex = 0;
			for(unsigned int i=0; i<solidBlocks.size(); i++) {
				if(solidBlocks[i].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
					if(tempIndex == powerupIndex) {
						// set powerup position
						Powerup p;
						p.SetPowerupType(FlameP);
						p.SetPosition(solidBlocks[i].GetPosition());
						p.SetScale(solidBlocks[i].GetScale());
						
						powerups.push_back(p);
						break;
					}
					
					tempIndex++;
				}
			}
			
			freee = true;
		} while (freee != true);
	}
	
	// generate bomb powerups
	for(int i=0; i<map.GetBombPowerups(); i++) {
		bool freee = false;
		do {
			int powerupIndex = rand() % maxIndex;	
			
			if(find(takenNodes.begin(), takenNodes.end(), powerupIndex) != takenNodes.end())
				continue;
			
			takenNodes.push_back(powerupIndex);
			
			int tempIndex = 0;
			for(unsigned int i=0; i<solidBlocks.size(); i++) {
				if(solidBlocks[i].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
					if(tempIndex == powerupIndex) {
						// set powerup position
						Powerup p;
						p.SetPowerupType(BombP);
						p.SetPosition(solidBlocks[i].GetPosition());
						p.SetScale(solidBlocks[i].GetScale());
						
						powerups.push_back(p);
						break;
					}
					
					tempIndex++;
				}
			}
			
			freee = true;
		} while (freee != true);
	}
	
	// generate speed powerups
	for(int i=0; i<map.GetSpeedPowerups(); i++) {
		bool freee = false;
		do {
			int powerupIndex = rand() % maxIndex;	
			
			if(find(takenNodes.begin(), takenNodes.end(), powerupIndex) != takenNodes.end())
				continue;
			
			takenNodes.push_back(powerupIndex);
			
			int tempIndex = 0;
			for(unsigned int i=0; i<solidBlocks.size(); i++) {
				if(solidBlocks[i].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
					if(tempIndex == powerupIndex) {
						// set powerup position
						Powerup p;
						p.SetPowerupType(SpeedP);
						p.SetPosition(solidBlocks[i].GetPosition());
						p.SetScale(solidBlocks[i].GetScale());
						
						powerups.push_back(p);
						break;
					}
					
					tempIndex++;
				}
			}
			
			freee = true;
		} while (freee != true);
	}
	
	// generate remote powerups
	for(int i=0; i<map.GetRemotePowerups(); i++) {
		bool freee = false;
		do {
			int powerupIndex = rand() % maxIndex;	
			
			if(find(takenNodes.begin(), takenNodes.end(), powerupIndex) != takenNodes.end())
				continue;
			
			takenNodes.push_back(powerupIndex);
			
			int tempIndex = 0;
			for(unsigned int i=0; i<solidBlocks.size(); i++) {
				if(solidBlocks[i].GetTextureID() == LEVEL_BLOCK_EXPLODABLE) {
					if(tempIndex == powerupIndex) {
						// set powerup position
						Powerup p;
						p.SetPowerupType(RemoteP);
						p.SetPosition(solidBlocks[i].GetPosition());
						p.SetScale(solidBlocks[i].GetScale());
						
						powerups.push_back(p);
						break;
					}
					
					tempIndex++;
				}
			}
			
			freee = true;
		} while (freee != true);
	}
}

void Level::OnWindowResized() {
	float w = (float)glutGet(GLUT_WINDOW_WIDTH);
	float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
	
	topBar = Sprite(Vector2(0,h), Vector2(w, 25), 0);
	topBar.SetColor(0.2f,0.2f,0.2f);
	endGame = Sprite(Vector2(0,h/3*2), Vector2(w, h/3), 30);
	endGameP1 = Sprite(Vector2(0,h/3*2), Vector2(w, h/3), 31);
	endGameP2 = Sprite(Vector2(0,h/3*2), Vector2(w, h/3), 32);
	endGameD = Sprite(Vector2(0,h/3*2), Vector2(w, h/3), 33);
	
	// clear map
	backgroundBlocks.clear();
	solidBlocks.clear();
	otherBlocks.clear();
	
	Vector2 boundaries(w, h-25);
	
	float xCount = map.GetX();
	float yCount = map.GetY();
	float xOffset = 0;
	float yOffset = 0;
	
	// calculate block size and offsets
	float blockSize = boundaries.Y() / yCount;
	if(blockSize * xCount <= boundaries.X()) {
		xOffset = (boundaries.X() - (blockSize * xCount)) / 2;
	}
	else {
		blockSize = boundaries.X() / xCount;
		yOffset = (boundaries.Y() - (blockSize * yCount)) / 2;
	}
	
	int c = 0;
	for(int i = 0; i < map.GetY(); i++) {
		for(int j = 0; j < map.GetX(); j++) {
			int texture = LEVEL_BLOCK_BACKGROUND;
			bool drawStatic = false;
			bool drawNothing = false;
			// switch to determine texture, block type accessed by map[c++]
			int b = map[c++];
			switch(b) {
				case BLOCK_SOLID:
					texture = LEVEL_BLOCK_SOLID;
					drawStatic = true;
					break;
				case BLOCK_EXPLODABLE:
					texture = LEVEL_BLOCK_EXPLODABLE;
					drawStatic = true;
					break;
				case BLOCK_PLAYER:
					if(loaded) {
						if(!map.GetSinglePlayer()) {
							p1 = Player(PLAYER_ONE, PlayerCharacter, blockSize/15, 5, Vector2(xOffset+j*blockSize+blockSize/8, boundaries.Y()-i*blockSize-yOffset-blockSize/8),
								Vector2(blockSize*6/8, blockSize*6/8), 14, PIVOT_TOPLEFT);
							p1.Start();
						}
						break;
					}
					p2 = Player(PLAYER_TWO, PlayerCharacter, blockSize/15, 5, Vector2(xOffset+j*blockSize+blockSize/8, boundaries.Y()-i*blockSize-yOffset-blockSize/8),
						Vector2(blockSize*6/8, blockSize*6/8), 14, PIVOT_TOPLEFT);
					p2.Start();
					//cout << "LOADED\n";
					drawNothing = true;
					loaded = true;
					break;
				default:
					drawNothing = true;
					if(b >= 0 && b <= 9) {
						Enemy e(EnemyCharacter, blockSize/15, 5, Vector2(xOffset+j*blockSize+blockSize/8, boundaries.Y()-i*blockSize-yOffset-blockSize/8),
							Vector2(blockSize*6/8, blockSize*6/8), 15, PIVOT_TOPLEFT);
						e.Start();
						
						ai.push_back(e);
					}
					break;
			}
			
			// draw background
			backgroundBlocks.push_back(
					Sprite(
						Vector2(xOffset+j*blockSize, boundaries.Y()-i*blockSize-yOffset),
						Vector2(blockSize, blockSize),
						LEVEL_BLOCK_BACKGROUND
					)
				);
			
			if(drawStatic && !drawNothing)
				solidBlocks.push_back(
					Sprite(
						Vector2(xOffset+j*blockSize, boundaries.Y()-i*blockSize-yOffset),
						Vector2(blockSize, blockSize),
						drawStatic ? texture : LEVEL_BLOCK_BACKGROUND
					)
				);
			
			if(!drawStatic)
				otherBlocks.push_back(
					Sprite(
						Vector2(xOffset+j*blockSize, boundaries.Y()-i*blockSize-yOffset),
						Vector2(blockSize, blockSize),
						texture
					)
				);
		}
	}
}

void Level::KeyPress(unsigned char key) {
	// escape to exit to main menu
	if(key == 27) {
		SwitchScene(SCENE_MAIN_MENU);
	}
		
	p2.KeyPress(key);
	
}

void Level::SpecialKeyPress(int key) {
	p1.SpecialKeyPress(key);
}

void Level::KeyRelease(unsigned char key) {
	p2.KeyRelease(key);
	
	// p2 bomb plant
	if(key == 'a' && (unsigned int)p2.GetBombCount() > bombs1.size() && p2.IsAlive()) {
		Bomb b;
		b.SetPosition(p2.GetPosition());
		b.SetScale(p2.GetScale());
		b.SetRemoteFireEnabled(p2.GetRemoteFIreEnabled());
		b.SetRange(p2.GetBombRange());
		
		Vector2 closest(-1,-1);
		if(backgroundBlocks.size() >0) {
			for(unsigned int i=0; i<backgroundBlocks.size(); i++) {
				float c, j;
				c = sqrt(pow(fabs(b.GetPosition().X()-closest.X()), 2) + pow(fabs(b.GetPosition().Y()-closest.Y()), 2));
				j = sqrt(pow(fabs(b.GetPosition().X()-backgroundBlocks[i].GetPosition().X()), 2) + pow(fabs(b.GetPosition().Y()-backgroundBlocks[i].GetPosition().Y()), 2));
				
				if(j < c) closest = backgroundBlocks[i].GetPosition();
			}
		}
		b.SetPosition(Vector2(closest.X() + p2.GetScale().X()/6, closest.Y() - p2.GetScale().Y()/6));
		bombs1.push_back(b);
	}
	if(key == 'q' && bombs1.size() > 0 && p2.IsAlive()) {
		bombs1[0].SetFire(true);
	}
	
	// p1 bomb plant
	if(map.GetSinglePlayer())
		return;
	if(key == 'l' && (unsigned int)p1.GetBombCount() > bombs2.size() && p1.IsAlive()) {
		cout << "\nPLANTING";
		Bomb b;
		b.SetPosition(p1.GetPosition());
		b.SetScale(p1.GetScale());
		b.SetRemoteFireEnabled(p1.GetRemoteFIreEnabled());
		b.SetRange(p1.GetBombRange());
		
		Vector2 closest(-1,-1);
		if(backgroundBlocks.size() >0) {
			for(unsigned int i=0; i<backgroundBlocks.size(); i++) {
				float c, j;
				c = sqrt(pow(fabs(b.GetPosition().X()-closest.X()), 2) + pow(fabs(b.GetPosition().Y()-closest.Y()), 2));
				j = sqrt(pow(fabs(b.GetPosition().X()-backgroundBlocks[i].GetPosition().X()), 2) + pow(fabs(b.GetPosition().Y()-backgroundBlocks[i].GetPosition().Y()), 2));
				
				if(j < c) closest = backgroundBlocks[i].GetPosition();
			}
		}
		b.SetPosition(Vector2(closest.X() + p1.GetScale().X()/6, closest.Y() - p1.GetScale().Y()/6));
		bombs2.push_back(b);
	}
	if(key == 'k' && bombs2.size() > 0 && p1.IsAlive()) {
		bombs2[0].SetFire(true);
	}
}

void Level::SpecialKeyRelease(int key) {
	p1.SpecialKeyRelease(key);
}