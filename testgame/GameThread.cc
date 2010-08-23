/*
 *  GameThread.cc
 *  testgame
 *
 *  Created by Steve Barnett on 21/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#include "GameThread.h"

#include "MSTimer.h"
#include "MSInput.h"
#include "MSRender.h"

#include <stdio.h>

#include "Entity.h"
#include "AssetManager.h"

GameThread::GameThread()
: MSThread()
{
	AM::LoadResources();
	srand( MSTimer::GetTime() );
}

GameThread::~GameThread()
{
	AM::UnloadResources();
}

void GameThread::RunThread()
{
	while ( !ExitRequested() )
	{
		m_game.Update();
	}
	Exit();
}

