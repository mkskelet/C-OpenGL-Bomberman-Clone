#Author: Marek Kost
#kostmare@fit.cvut.cz

#allowed commands: rm, gcc, g++, mkdir, doxygen, cp, mv, cd, ar, make
CXXFLAGS=-O0 -lglut -lGLU -lGL -Wall
CLIBS=
CXX=g++
RM=rm -rf
OBJECTS=bin/objects/Bomb.o bin/objects/Character.o bin/objects/Enemy.o bin/objects/Explosion.o bin/objects/GameTime.o bin/objects/GameWindow.o bin/objects/Level.o bin/objects/main.o bin/objects/MainMenu.o bin/objects/Map.o bin/objects/MapLoader.o bin/objects/Player.o bin/objects/PlayerPrefs.o bin/objects/Portal.o bin/objects/Powerup.o bin/objects/Scene.o bin/objects/SplashScreen.o bin/objects/Sprite.o bin/objects/tga.o bin/objects/Vector2.o
DOXY=Doxyfile
BIN=./kostmare
#end of macros ------------------------------------------------------------------------------------------

#generates final binary and documentation
all: $(OBJECTS) $(DOXY)
	make compile
	make doc

#build all objects and merge them into final binary
compile: $(BIN)

#run program
run: $(BIN)
	$(BIN) test #with no parameter return code 1 is returned, param "test" returns 0

#delete all folders with binary files (including documentation)
clean:
	$(RM) bin doc $(BIN)

#generate documentation in '<login>/doc' folder
doc: $(DOXY) src/*
	( cd src | doxygen $(DOXY) 2> /dev/null > /dev/null )

#rules to make objects ----------------------------------------------------------------------------------
$(BIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BIN) $(CLIBS)

bin/objects/Bomb.o: src/Bomb.cpp src/Bomb.h src/Sprite.h src/Vector2.h src/GameTime.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/Bomb.cpp -o bin/objects/Bomb.o $(CLIBS)

bin/objects/Character.o: src/Character.cpp src/Character.h src/Sprite.h src/Vector2.h src/Explosion.h src/GameTime.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/Character.cpp -o bin/objects/Character.o $(CLIBS)

bin/objects/Enemy.o: src/Enemy.cpp src/Enemy.h src/Character.h src/Sprite.h src/Vector2.h src/Explosion.h src/GameTime.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/Enemy.cpp -o bin/objects/Enemy.o $(CLIBS)

bin/objects/Explosion.o: src/Explosion.cpp src/Explosion.h src/Sprite.h src/Vector2.h src/GameTime.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/Explosion.cpp -o bin/objects/Explosion.o $(CLIBS)

bin/objects/GameTime.o: src/GameTime.cpp src/GameTime.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/GameTime.cpp -o bin/objects/GameTime.o $(CLIBS)

bin/objects/GameWindow.o: src/GameWindow.cpp src/GameWindow.h src/MainMenu.h src/Scene.h src/tga.h \
 src/Sprite.h src/Vector2.h src/MapLoader.h src/Map.h src/SplashScreen.h src/GameTime.h src/Level.h \
 src/Player.h src/Character.h src/Explosion.h src/Enemy.h src/Portal.h src/Powerup.h src/Bomb.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/GameWindow.cpp -o bin/objects/GameWindow.o $(CLIBS)

bin/objects/Level.o: src/Level.cpp src/Level.h src/Scene.h src/MapLoader.h src/Map.h src/Sprite.h src/Vector2.h \
 src/Player.h src/Character.h src/Explosion.h src/GameTime.h src/Enemy.h src/Portal.h src/Powerup.h \
 src/Bomb.h src/PlayerPrefs.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/Level.cpp -o bin/objects/Level.o $(CLIBS)

bin/objects/main.o: src/main.cpp src/GameWindow.h src/MainMenu.h src/Scene.h src/tga.h src/Sprite.h src/Vector2.h \
 src/MapLoader.h src/Map.h src/SplashScreen.h src/GameTime.h src/Level.h src/Player.h src/Character.h \
 src/Explosion.h src/Enemy.h src/Portal.h src/Powerup.h src/Bomb.h src/PlayerPrefs.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o bin/objects/main.o $(CLIBS)

bin/objects/MainMenu.o: src/MainMenu.cpp src/MainMenu.h src/Scene.h src/tga.h src/Sprite.h src/Vector2.h \
 src/MapLoader.h src/Map.h src/Level.h src/Player.h src/Character.h src/Explosion.h src/GameTime.h \
 src/Enemy.h src/Portal.h src/Powerup.h src/Bomb.h src/PlayerPrefs.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/MainMenu.cpp -o bin/objects/MainMenu.o $(CLIBS)

bin/objects/Map.o: src/Map.cpp src/Map.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/Map.cpp -o bin/objects/Map.o $(CLIBS)

bin/objects/MapLoader.o: src/MapLoader.cpp src/MapLoader.h src/Map.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/MapLoader.cpp -o bin/objects/MapLoader.o $(CLIBS)

bin/objects/Player.o: src/Player.cpp src/Player.h src/Character.h src/Sprite.h src/Vector2.h src/Explosion.h \
 src/GameTime.h src/Enemy.h src/Portal.h src/Powerup.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/Player.cpp -o bin/objects/Player.o $(CLIBS)

bin/objects/PlayerPrefs.o: src/PlayerPrefs.cpp src/PlayerPrefs.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/PlayerPrefs.cpp -o bin/objects/PlayerPrefs.o $(CLIBS)

bin/objects/Portal.o: src/Portal.cpp src/Portal.h src/Sprite.h src/Vector2.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/Portal.cpp -o bin/objects/Portal.o $(CLIBS)

bin/objects/Powerup.o: src/Powerup.cpp src/Powerup.h src/Sprite.h src/Vector2.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/Powerup.cpp -o bin/objects/Powerup.o $(CLIBS)

bin/objects/Scene.o: src/Scene.cpp src/Scene.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/Scene.cpp -o bin/objects/Scene.o $(CLIBS)

bin/objects/SplashScreen.o: src/SplashScreen.cpp src/SplashScreen.h src/Scene.h src/GameTime.h src/tga.h \
 src/Sprite.h src/Vector2.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/SplashScreen.cpp -o bin/objects/SplashScreen.o $(CLIBS)

bin/objects/Sprite.o: src/Sprite.cpp src/Sprite.h src/Vector2.h src/tga.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/Sprite.cpp -o bin/objects/Sprite.o $(CLIBS)

bin/objects/tga.o: src/tga.cpp src/tga.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/tga.cpp -o bin/objects/tga.o $(CLIBS)

bin/objects/Vector2.o: src/Vector2.cpp src/Vector2.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/Vector2.cpp -o bin/objects/Vector2.o $(CLIBS)
