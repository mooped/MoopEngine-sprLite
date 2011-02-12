/*
 *  GameThread.h
 *  bunny chicken dinosaur
 *
 *  Created by Steve Barnett on 12/02/2011.
 *  Copyright 2011 Stephen Barnett. All rights reserved.
 *
 */

#ifndef GAMETHREAD_DEF
#define GAMETHREAD_DEF

#include "MSEngine.h"

#include "MSThread.h"

#include "Game.h"

class GameThread : public MSThread
{
private:
	Game m_game;

public:
	GameThread();
	virtual ~GameThread();
	virtual void RunThread( void );
};

#endif

