/*
 *  GameThread.cc
 *	bunny chicken dinosaur 
 *
 *  Created by Steve Barnett on 12/02/2011.
 *  Copyright 2011 Stephen Barnett. All rights reserved.
 *
 */

#include "GameThread.h"

#include "MSTimer.h"
#include "MSInput.h"
#include "MSRender.h"

#include <stdio.h>

GameThread::GameThread()
: MSThread()
{
}

GameThread::~GameThread()
{
}

void GameThread::RunThread()
{
	while ( !ExitRequested() )
	{
		m_game.Update();
		if ( m_game.ShouldQuit() ) break;
	}
	Exit();
}

