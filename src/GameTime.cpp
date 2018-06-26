///
/// Class saves time(in miliseconds) since game was started and allows access to other classes for timing purposes.
///

#include "GameTime.h"

long GameTime::time = 0;

GameTime::GameTime() {
	
}

void GameTime::AddTime(int milis) {
	time += milis;
}

long GameTime::GetTime() {
	return time;
}