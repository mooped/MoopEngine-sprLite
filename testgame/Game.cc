/*
 *  Game.cc
 *  testgame
 *
 *  Created by Steve Barnett on 22/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Game.h"

#include "MSTimer.h"
#include "MSInput.h"
#include "MSRender.h"

#include <stdio.h>

#include "Entity.h"
#include "AssetManager.h"

Game::Game()
: nextTick( 0 )
{
	AM::LoadResources();
	srand( MSTimer::GetTime() );
}

Game::~Game()
{
	AM::UnloadResources();
}

void Game::Begin()
{
	Entity::BeginGame();
}

void Game::Update()
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
		return;
	}

	MSTimer::FrameStart();
	MSInput::ResetFrameKeys();

	MSRender::ClearColour( 0x000000ff );;
	MSRender::BeginScene();

	Entity::Update();
	Entity::Render();

	MSRender::EndScene();
	MSRender::Sync();
}


