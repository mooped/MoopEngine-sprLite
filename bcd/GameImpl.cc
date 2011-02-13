#include "MSEngine.h"

#include "GameImpl.h"

#include "MSSprite.h"
#include "MSRender.h"

#include "Setup.h"

#include <stdio.h>
#include <stdlib.h>

#define PER_SCREEN (640 / 64)

GameImpl::GameImpl()
: m_position( 0 )
, m_stage( 0 )
{
	fprintf( stderr, "GameImpl created\n" );

	for ( int i = 0; i < LEVEL_LEN; ++i )
	{
		m_heights[i] = rand() % 4 + 1;
		m_backdrop[i] = rand() % 6;
	}
	int largehit = 0;
	for ( int i = 0; i < LEVEL_LEN; ++i )
	{
		switch ( largehit )
		{
			case 1:
			{
				--largehit;
				m_backdrop[i] = 2;
			} break;
			case 2:
			{
				--largehit;
				m_backdrop[i] = 1;
			} break;
			default:
			{
				if ( m_backdrop[i] <= 2 )
				{
					largehit = 2;
					m_backdrop[i] = 0;
				}
			} break;
		}
	}
}

GameImpl::~GameImpl()
{
	fprintf( stderr, "GameImpl deleted\n" );
}

void GameImpl::Update()
{
	fprintf( stderr, "GameImpl update\n" );

	++m_position;

	if ( m_position > ( LEVEL_LEN * 32 ) - ( PER_SCREEN + 1 ) )
	{
		m_position = 0;
	}
}

void GameImpl::Render()
{
	static int starttile = 0;

	Setup::STweaks tweaks = Setup::GetTweaks();
	Setup::SStage stage = Setup::GetStage( m_stage );

	// Sky
	MSRender::ClearColour( stage.sky );

	// Background
	static Setup::ESprites bg[6] =
	{
		Setup::eSp_Large1,
		Setup::eSp_Large2,
		Setup::eSp_Large3,
		Setup::eSp_Small1,
		Setup::eSp_Small2,
		Setup::eSp_Small3,
	};

	for ( int i = 0; i <= PER_SCREEN; ++i )
	{
		const int tile = m_backdrop[i + starttile];
		if ( tile >= 0 && tile <= 5 )
		{
			Setup::ESprites id = bg[tile];
			MSSprite::RenderSprite( Setup::Sheet( id ), Setup::Sprite( id ), MSVec( i * 64 + 32 - ( m_position % 64 ), 96 ), 6, MSVec( 64, 192 ), stage.bg );
		}
	}

	// Foreground
	static Setup::ESprites fg[4] =
	{
		Setup::eSp_Kerb2,
		Setup::eSp_Low,
		Setup::eSp_Mid,
		Setup::eSp_High2,
	};

	for ( int i = 0; i <= PER_SCREEN; ++i )
	{
		const int height = m_heights[i + starttile];
		if ( height > 0 && height < 5 )
		{
			Setup::ESprites id = fg[height - 1];
			MSSprite::RenderSprite( Setup::Sheet( id ), Setup::Sprite( id ), MSVec( i * 64 + 32 - ( m_position % 64 ), 96 ), 5, MSVec( 64, 192 ), stage.fg );
		}
	}

	// Scrolling (yuck!)
	if ( m_position % 64 == 63 )
	{
		++starttile;
	}
}

void GameImpl::Reset()
{
	fprintf( stderr, "GameImpl reset\n" );
}

