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
#include "MSRender.h"
#include "MSFont.h"
#include "MSSprite.h"

#include <stdio.h>

GameThread::GameThread()
: MSThread()
, m_font( new MSImage( "../../charset.tga" ) )
, m_sprites( new MSImage( "../../sprites.tga" ) )
{
	MSFont::Initialise( "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!?.:'c", m_font, MSVec( 6, 8 ) );
	m_shipSheet = MSSprite::AddSheet( m_sprites, MSVec( 16, 8 ) );
}

GameThread::~GameThread()
{
	delete m_font;
	delete m_sprites;
}

void GameThread::RunThread()
{
	while ( !ExitRequested() )
	{
		MSTimer::FrameStart();

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
	MSSprite::RenderSprite( m_shipSheet, i++, MSVec( 64, 64 ), 4, MSVec( 32, 16 ), 0xffffffff );
	MSSprite::RenderSprite( m_shipSheet, 8, MSVec( 96, 96 ), 4, MSVec( 32, 16 ), 0xffffffff );
	MSSprite::RenderSprite( m_shipSheet, 16, MSVec( 128, 128 ), 4, MSVec( 32, 16 ), 0xffffffff );
	MSSprite::RenderSprite( m_shipSheet, 24, MSVec( 96, 160 ), 4, MSVec( 32, 16 ), 0xffffffff );
	MSSprite::RenderSprite( m_shipSheet, 32, MSVec( 128, 192 ), 4, MSVec( 32, 16 ), 0xffffffff );
	MSRender::EndScene();
	MSRender::Sync();
}

