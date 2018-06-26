///
/// Class saves time(in miliseconds) since game was started and allows access to other classes for timing purposes.
///

#ifndef GameTime_h
#define GameTime_h

#include <GL/glut.h>

class GameTime {
public:
	static long time;	///< time in miliseconds
	GameTime();
	static void AddTime(int milis);		///< Adds time.
	static long GetTime();				///< Returns time since game started (in miliseconds).
};

#endif // GameTime_h