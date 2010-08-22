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
#include "MSFont.h"

#include <stdio.h>

#include "Entity.h"
#include "AssetManager.h"

Game::Game()
: nextTick( 0 )
, state( eS_Title )
, score( 0 )
{
	AM::LoadResources();
	srand( MSTimer::GetTime() );
}

Game::~Game()
{
	AM::UnloadResources();
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

	switch ( state )
	{
		case eS_Title:
		{
			MSFont::RenderString( "ASTROVAX", MSVec( 320, 64 ), 5, MSVec( 32, 32 ), 0xff2f2fff, true );
			MSFont::RenderString( "BY MOOP FOR LD:18", MSVec( 320, 80 ), 5, MSVec( 16, 16 ), 0xffffffff, true );
			MSFont::RenderString( "DEFEND THE ASTROVAC", MSVec( 320, 100 ), 5, MSVec( 16, 16 ), 0xffffffff, true );

			MSFont::RenderString( "SPACE : FIRE               ", MSVec( 320, 160 ), 5, MSVec( 16, 16 ), 0xffffffff, true );
			MSFont::RenderString( "    Q : SUCK               ", MSVec( 320, 180 ), 5, MSVec( 16, 16 ), 0xffffffff, true );
			MSFont::RenderString( "    E : POWERUP OR JETTISON", MSVec( 320, 200 ), 5, MSVec( 16, 16 ), 0xffffffff, true );
			MSFont::RenderString( " WSAD : MOVE               ", MSVec( 320, 220 ), 5, MSVec( 16, 16 ), 0xffffffff, true );

			MSFont::RenderString( "SUCK UP STUNNED ENEMIES", MSVec( 320, 260 ), 5, MSVec( 16, 16 ), 0xffffffff, true );
			MSFont::RenderString( "TURN MATCHING SETS INTO POWEUPS", MSVec( 320, 290 ), 5, MSVec( 16, 16 ), 0xffffffff, true );
			MSFont::RenderString( "JETTISON MISMATCHED ENEMIES", MSVec( 320, 320 ), 5, MSVec( 16, 16 ), 0xffffffff, true );
			MSFont::RenderString( "ELEMENTAL KILLS SCORE EXTRA POINTS", MSVec( 320, 350 ), 5, MSVec( 16, 16 ), 0xffffffff, true );

			MSFont::RenderString( "HIT FIRE TO PLAY", MSVec( 320, 380 ), 5, MSVec( 16, 16 ), 0xff2f2fff, true );

			MSFont::RenderString( "SMALL PRINT: THE ASTROVAC IS TO THE LEFT, I HAD NO TIME TO DRAW IT", MSVec( 320, 468 ), 5, MSVec( 8, 8 ), 0xff2f2fff, true );
			MSFont::RenderString( "USE IMAGINATION!", MSVec( 320, 475 ), 5, MSVec( 8, 8 ), 0xff2f2fff, true );

			if ( MSInput::Key( ' ' ) )
			{
				MSInput::ResetKey( ' ' );
				Entity::BeginGame();
				state = eS_Game;
			}
		} break;
		case eS_Game:
		{
			Entity::Update();

			if ( Entity::IsGameOver() )
			{
				Entity::Render();
				score = Entity::GetScore();
				state = eS_Score;
				Entity::FinishGame();
				MSInput::ResetKey( ' ' );
			}
			else
			{
				Entity::Render();
			}
		} break;
		case eS_Score:
		{
			MSFont::RenderString( "GAME OVER", MSVec( 320, 64 ), 5, MSVec( 32, 32 ), 0xff2f2fff, true );

			char pscore[64];
			snprintf( pscore, 64, "SCORE:%i", score );
			MSFont::RenderString( pscore, MSVec( 320, 150 ), 5, MSVec( 40, 40 ), 0xffffffff, true );

			MSFont::RenderString( "PRESS FIRE TO CONTINUE", MSVec( 320, 250 ), 5, MSVec( 24, 24 ), 0xffffffff, true );

			if ( MSInput::Key( ' ' ) )
			{
				MSInput::ResetKey( ' ' );
				state = eS_Title;
			}
		} break;
		default: break;
	}

	MSRender::EndScene();
	MSRender::Sync();
}


