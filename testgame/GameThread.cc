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
#include "MSFont.h"
#include "MSSprite.h"

#include <stdio.h>

#include "AssetManager.h"

GameThread::GameThread()
: MSThread()
{
	AM::LoadResources();
}

GameThread::~GameThread()
{
	AM::UnloadResources();
}

void GameThread::RunThread()
{
	while ( !ExitRequested() )
	{
		MSTimer::FrameStart();
		MSInput::ResetFrameKeys();

		GameUpdate();
	}
	Exit();
}

void GameThread::GameUpdate()
{
	static int i = 0;
	MSRender::ClearColour( 0x000000ff );;
	MSRender::BeginScene();
	MSFont::RenderString( "HELLO WORLD!", MSVec( 32, 32 ), 5, MSVec( 24, 32 ), 0xffff00ff );
	if ( i > 4 ) i = 0;
	MSSprite::RenderSprite( AM::Ships(), i++, MSVec( 64, 64 ), 4, MSVec( 32, 16 ));
	MSSprite::RenderSprite( AM::Ships(), 8, MSVec( 96, 96 ), 4, MSVec( 32, 16 ));
	MSSprite::RenderSprite( AM::Ships(), 16, MSVec( 128, 128 ), 4, MSVec( 32, 16 ));
	MSSprite::RenderSprite( AM::Ships(), 24, MSVec( 96, 160 ), 4, MSVec( 32, 16 ));
	MSSprite::RenderSprite( AM::Ships(), 32, MSVec( 128, 192 ), 4, MSVec( 32, 16 ));
	MSRender::EndScene();
	MSRender::Sync();
}

