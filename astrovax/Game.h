/*
 *  Game.h
 *  testgame
 *
 *  Created by Steve Barnett on 22/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef GAME_DEF
#define GAME_DEF

#include "MSEngine.h"

class Game
{
private:
	enum EState
	{
		eS_Title = 0,
		eS_Game,
		eS_Score,
		eS_Count,
	};

	int nextTick;
	EState state;
	int score;

public:
	Game();
	~Game();

	void Update();
};

#endif

