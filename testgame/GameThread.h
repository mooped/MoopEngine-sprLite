/*
 *  GameThread.h
 *  testgame
 *
 *  Created by Steve Barnett on 21/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GAMETHREAD_DEF
#define GAMETHREAD_DEF

#include "MSEngine.h"

#include "MSThread.h"

class GameThread : public MSThread
{
private:
	int nextTick;

protected:
	void GameSetup();
	void GameUpdate();

public:
	GameThread();
	virtual ~GameThread();
	virtual void RunThread( void );
};

#endif
