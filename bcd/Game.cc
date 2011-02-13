/*
 *  Game.cc
 *  bunny chicken dinosaur
 *
 *  Created by Steve Barnett on 12/02/2011.
 *  Copyright 2011 Stephen Barnett. All rights reserved.
 *
 */

#include "Game.h"

#include "MSTimer.h"
#include "MSInput.h"
#include "MSRender.h"
#include "MSFont.h"
#include "MSSprite.h"
#include "MSTile.h"
#include "MSMenu.h"

#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#define sleep Sleep
#endif

#include "Setup.h"

#include "GameImpl.h"

static void ButtonCallback( void* ptr )
{
	*reinterpret_cast<bool*>(ptr) = true;
}

struct SGameMenu
{
	SGameMenu()
	{
		Reset();
	}
	void Reset()
	{
		m_resumeRequested = false;
		m_resetRequested = false;
		m_menuRequested = false;
		m_quitRequested = false;
	}

	void RegisterFields( MSMenu& menu )
	{
		menu.RegisterField( "RESUME", &ButtonCallback, reinterpret_cast<void*>( &m_resumeRequested ) );
		menu.RegisterField( "RESTART", &ButtonCallback, reinterpret_cast<void*>( &m_resetRequested ) );
		menu.RegisterField( "QUIT TO MENU", &ButtonCallback, reinterpret_cast<void*>( &m_menuRequested ) );
		//menu.RegisterField( "QUIT", &ButtonCallback, reinterpret_cast<void*>( &m_quitRequested ) );
	}

	bool m_resumeRequested;
	bool m_resetRequested;
	bool m_menuRequested;
	bool m_quitRequested;
};

Game::Game()
: m_quit( 0 )
, m_nextTick( 0 )
, m_state( eS_Title )
,	m_pMenu( NULL )
, m_pGameMenu( new SGameMenu )
{
	srand( MSTimer::GetTime() );
	Setup::LoadResources();
	MSRender::ClearColour( 0x000000ff );
}

Game::~Game()
{
	delete m_pMenu;
	delete m_pGameMenu;

	// Unload resources
	Setup::UnloadResources();
}

void Game::Update()
{
	// Lock to ~60fps
	const int time = MSTimer::GetTime();
	if ( time > m_nextTick )
	{
		m_nextTick = time + 16;
	}
	else
	{
		sleep( 0 );
		return;
	}

	MSTimer::FrameStart();
	MSInput::ResetFrameKeys();

	MSRender::BeginScene();

	switch ( m_state )
	{
		case eS_Title:
		{
			UpdateTitle();
		} break;
		case eS_Game:
		{
			UpdateGame();
		} break;
		case eS_GameMenu:
		{
			UpdateGameMenu();
		} break;
		case eS_Victory:
		{
			UpdateVictory();
		} break;
		case eS_Failure:
		{
			UpdateFailure();
		} break;
		default: break;
	}

	MSRender::EndScene();
	MSRender::Sync();
}

void Game::UpdateTitle()
{
	MSFont::RenderString( "BUNNY CHICKEN DINOSAUR", MSVec( 320, 50 ), 5, MSVec( 24, 24 ), 0xffffffff, true );
	MSFont::RenderString( "HELP A BUNNY A CHICKEN AND A DINOSAUR", MSVec( 320, 100 ), 5, MSVec( 16, 16 ), 0xffffffff, true );
	MSFont::RenderString( "ESCAPE TO A PLACE THAT WILL ACCEPT", MSVec( 320, 130 ), 5, MSVec( 16, 16 ), 0xffffffff, true );
	MSFont::RenderString( "THEIR UNORTHODOX LOVE", MSVec( 320, 160 ), 5, MSVec( 16, 16 ), 0xffffffff, true );

	MSFont::RenderString( "PRESS SPACE", MSVec( 320, 400 ), 5, MSVec( 32, 32 ), 0xffffffff, true );

	MSFont::RenderString( "COPYRIGHT ` 2010 STEVE BARNETT. ALL RIGHTS RESERVED.", MSVec( 320, 470 ), 5, MSVec( 10, 10 ), 0xffffffff, true );

	MSSprite::RenderSprite( Setup::Sheet( Setup::eSp_Title, 0 ), Setup::Sprite( Setup::eSp_Title, 0 ), MSVec( 320, 240 ), 5, MSVec( 256, 256 ), 0xffffffff );

	if ( MSInput::Key( ' ' ) )
	{
		MSInput::ResetKey( ' ' );
		m_pauseState = eS_GameMenu;
		m_state = eS_Game;
	}
}

void Game::UpdateGame()
{
	// Update the game
	if ( !m_pGame )
	{
		m_pGame = new GameImpl();
	}

	m_pGame->Update();
	m_pGame->Render();

	if ( m_pGame->Victory() )
	{
		m_state = eS_Victory;
		delete m_pGame;
		m_pGame = NULL;
	}
	else if ( m_pGame->Failure() )
	{
		m_state = eS_Failure;
		delete m_pGame;
		m_pGame = NULL;
	}

	MSFont::RenderString( "Q : BUNNY JUMP", MSVec( 320, 400 ), 5, MSVec( 32, 32 ), 0xffffffff, true );
	MSFont::RenderString( "E : DINOSAUR JUMP", MSVec( 320, 440 ), 5, MSVec( 32, 32 ), 0xffffffff, true );

	if ( MSInput::Key( 27 ) ) // Esc
	{
		MSInput::ResetKey( 27 );
		m_state = m_pauseState;
	}
}

void Game::UpdateGameMenu()
{
	if ( !m_pMenu )
	{
		m_pMenu = new MSMenu();
		m_pMenu->RegisterField( "PAUSED:" );
		m_pGameMenu->RegisterFields( *m_pMenu );
	}

	m_pMenu->Update();
	m_pMenu->Render( MSVec( 32, 128 ) );

	MSFont::RenderString( "USE W AND A TO NAVIGATE THE MENU", MSVec( 320, 350 ), 5, MSVec( 24, 24 ), 0xffffffff, true );
	MSFont::RenderString( "PRESS SPACE", MSVec( 320, 400 ), 5, MSVec( 32, 32 ), 0xffffffff, true );

	MSFont::RenderString( "COPYRIGHT ` 2010 STEVE BARNETT. ALL RIGHTS RESERVED.", MSVec( 320, 470 ), 5, MSVec( 10, 10 ), 0xffffffff, true );

	MSSprite::RenderSprite( Setup::Sheet( Setup::eSp_Pause, 0 ), Setup::Sprite( Setup::eSp_Pause, 0 ), MSVec( 320, 240 ), 5, MSVec( 256, 256 ), 0xffffffff );

	if ( m_pGameMenu->m_resumeRequested )
	{
		delete m_pMenu;
		m_pMenu = NULL;
		m_pGameMenu->Reset();

		m_state = eS_Game;
	}
	else if ( m_pGameMenu->m_resetRequested )
	{
		delete m_pMenu;
		m_pMenu = NULL;
		m_pGameMenu->Reset();

		m_pGame->Reset();

		m_state = eS_Game;
	}
	else if ( m_pGameMenu->m_menuRequested )
	{
		delete m_pMenu;
		m_pMenu = NULL;
		m_pGameMenu->Reset();

		if ( m_pGame )
		{
			delete m_pGame;
			m_pGame = NULL;
		}

		m_state = eS_Title;
	}
	else if ( m_pGameMenu->m_quitRequested )
	{
		delete m_pMenu;
		m_pMenu = NULL;
		m_pGameMenu->Reset();

		if ( m_pGame )
		{
			delete m_pGame;
			m_pGame = NULL;
		}

		m_quit = true;
	}
}

void Game::UpdateVictory()
{
	MSSprite::RenderSprite( Setup::Sheet( Setup::eSp_Victory, 0 ), Setup::Sprite( Setup::eSp_Victory, 0 ), MSVec( 320, 240 ), 5, MSVec( 256, 256 ), 0xffffffff );

	MSFont::RenderString( "YOU MADE IT!", MSVec( 320, 100 ), 5, MSVec( 32, 32 ), 0xffffffff, true );
	MSFont::RenderString( "PRESS SPACE", MSVec( 320, 400 ), 5, MSVec( 32, 32 ), 0xffffffff, true );

	if ( MSInput::Key( ' ' ) )	// Space
	{
		MSInput::ResetKey( ' ' );
		m_state = eS_Title;
	}
}

void Game::UpdateFailure()
{
	MSSprite::RenderSprite( Setup::Sheet( Setup::eSp_Defeat, 0 ), Setup::Sprite( Setup::eSp_Defeat, 0 ), MSVec( 320, 240 ), 5, MSVec( 256, 256 ), 0xffffffff );

	MSFont::RenderString( "YOU TRIPPED AND FELL", MSVec( 320, 100 ), 5, MSVec( 24, 24 ), 0xffffffff, true );
	MSFont::RenderString( "DAYLIGHT IS FADING", MSVec( 320, 130 ), 5, MSVec( 16, 16 ), 0xffffffff, true );
	MSFont::RenderString( "BUT THERE IS ALWAYS TOMORROW", MSVec( 320, 156 ), 5, MSVec( 16, 16 ), 0xffffffff, true );
	MSFont::RenderString( "PRESS SPACE", MSVec( 320, 400 ), 5, MSVec( 32, 32 ), 0xffffffff, true );

	if ( MSInput::Key( ' ' ) )	// Space
	{
		MSInput::ResetKey( ' ' );
		m_state = eS_Title;
	}
}

