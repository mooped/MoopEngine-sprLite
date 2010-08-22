/*
 *  GameThread.cc
 *  testgame
 *
 *  Created by Steve Barnett on 21/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
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
, nextTick( 0 )
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
	Entity::BeginGame();

	while ( !ExitRequested() )
	{
		// Lock to ~60fps
		const int time = MSTimer::GetTime();
		if ( time > nextTick )
		{
			nextTick = time + 16;
		}
		else
		{
			sleep( 0 );
			continue;
		}

		MSTimer::FrameStart();
		MSInput::ResetFrameKeys();

		GameUpdate();
	}
	Exit();
}

void GameThread::GameUpdate()
{
	MSRender::ClearColour( 0x000000ff );;
	MSRender::BeginScene();

	Entity::Update();

	// Entity rendering 
	Entity::Render();

	MSRender::EndScene();
	MSRender::Sync();
}

