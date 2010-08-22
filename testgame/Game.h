/*
 *  Game.h
 *  testgame
 *
 *  Created by Steve Barnett on 22/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GAME_DEF
#define GAME_DEF

#include "MSEngine.h"

class Game
{
private:
	int nextTick;

public:
	Game();
	~Game();

	void Begin();
	void Update();
};

#endif

