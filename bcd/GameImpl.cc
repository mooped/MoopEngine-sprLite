#include "MSEngine.h"

#include "GameImpl.h"

#include "MSSprite.h"
#include "MSRender.h"
#include "MSInput.h"

#include "Setup.h"

#include <stdio.h>
#include <stdlib.h>

#define PER_SCREEN (640 / 64)

void Character::Update()
{
	++m_frame;
	if ( m_frame >= 3 ) m_frame = 0;

	if ( m_type == Setup::eCh_Chicken )
	{
		if ( m_frame == 0 )
		{
			m_pos = m_pos + m_vel;
			m_vel.y += ( m_vel.y > 0 ) ? 1 : -1;
			if ( m_vel.y > 5 || m_vel.y < -5 )
			{
				m_vel.y = ( m_vel.y > 0 ) ? -1 : 1;
			}
		}
	}
	else
	{
		m_vel = m_vel + MSVec( 0, 1 );
		m_pos = m_pos + m_vel;
	
		if ( m_airborne )
		{
			m_pos.x += 2;
		}
		else if ( m_pos.x > m_xtarget )
		{
			m_pos.x -= 2;
		}
	
		Setup::STweaks tweaks = Setup::GetTweaks();
	
		int vel;
		MSVec size;
		switch ( m_type )
		{
			case Setup::eCh_Bunny:
			{
				vel = tweaks.bunnyjump;
				size = MSVec( 40, 40 );
			} break;
			case Setup::eCh_Chicken:
			{
				vel = tweaks.chickenjump;
				size = MSVec( 32, 40 );
			} break;
			case Setup::eCh_Dinosaur:
			{
				vel = tweaks.dinosaurjump;
				size = MSVec( 72, 48 );
			} break;
		}
	
		const int level = m_pGame->ProbeHeight( m_pos.x + size.x / 2 );
	
		if ( m_pos.y > level )
		{
			if ( level - m_pos.y < 16 )
			{
				m_vel.y = 0;
				m_pos.y = level;
				m_airborne = false;
			}
			else
			{
				// D:
				exit( 0 );
			}
		}
	
		if ( MSInput::Key( m_key ) && !m_airborne )
		{
			m_vel.y -= vel;
			m_airborne = true;
		}
	}
}

void Character::Render()
{
	Setup::SCharData data = Setup::GetChar( m_type );
	Setup::STweaks tweaks = Setup::GetTweaks();

	unsigned int colour;
	MSVec size;
	switch ( m_type )
	{
		case Setup::eCh_Bunny:
		{
			colour = tweaks.bunnycolour;
			size = MSVec( 40, 40 );
		} break;
		case Setup::eCh_Chicken:
		{
			colour = tweaks.chickencolour;
			size = MSVec( 32, 40 );
		} break;
		case Setup::eCh_Dinosaur:
		{
			colour = tweaks.dinosaurcolour;
			size = MSVec( 72, 40 );
		} break;
	}

	MSSprite::RenderSprite( data.sheet, data.run[m_frame], m_pos - MSVec( 0, size.y), m_type + 1, size * 2, colour );
}

GameImpl::GameImpl()
: m_position( 0 )
, m_stage( 0 )
{
	fprintf( stderr, "GameImpl created\n" );

	int largehit = 0;
	for ( int i = 0; i < LEVEL_LEN; ++i )
	{
		if ( i < 8 || i > LEVEL_LEN - ( 2 * PER_SCREEN ) )
		{
			m_heights[i] = 0;
		}
		else
		{
			switch ( largehit )
			{
				case 1:
				case 2:
				{
					m_heights[i] = 4;
					--largehit;
				} break;
				default:
				{
					m_heights[i] = rand() % 4 + 1;
					if ( m_heights[i] - m_heights[i - 1] >= 2 )
					{
						--m_heights[i];
					}
					if ( m_heights[i] == 4 )
					{
						largehit = 2;
					}
				} break;
			}
		}
		m_backdrop[i] = rand() % 6;
	}
	largehit = 0;
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

	m_char[0] = Character( Setup::eCh_Bunny, 'q', this );
	m_char[1] = Character( Setup::eCh_Chicken, 'w', this );
	m_char[2] = Character( Setup::eCh_Dinosaur, 'e', this );
}

GameImpl::~GameImpl()
{
	fprintf( stderr, "GameImpl deleted\n" );
}

void GameImpl::Update()
{
	fprintf( stderr, "GameImpl update\n" );

	m_position += 2;

	if ( m_position > ( LEVEL_LEN * 64 ) - ( PER_SCREEN + 1 ) * 128 )
	{
		m_position = 0;
		m_stage += 1;
		if ( m_stage >= Setup::GetNumStages() )
		{
			m_stage = 0;
			// Victory
			exit( 0 );
		}
	}

	for ( int i = 0; i < NUM_CHARS; ++i )
	{
		m_char[i].Update();
	}
}

void GameImpl::Render()
{
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
		const int tile = ( m_position + i * 64 ) / 64;
		const int bgtile = m_backdrop[tile];
		if ( bgtile >= 0 && bgtile <= 5 )
		{
			Setup::ESprites id = bg[bgtile];
			MSSprite::RenderSprite( Setup::Sheet( id, m_stage ), Setup::Sprite( id, m_stage ), MSVec( i * 64 + 32 - ( m_position % 64 ), 96 ), 6, MSVec( 64, 192 ), stage.bg );
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
		const int tile = ( m_position + i * 64 ) / 64;
		const int height = m_heights[tile];
		if ( height > 0 && height < 5 )
		{
			Setup::ESprites id = fg[height - 1];
			if ( tile != 0 && id == Setup::eSp_High2 && m_heights[tile - 1] != 4 )
			{
				id = Setup::eSp_High1;
			}
			else if ( id == Setup::eSp_High2 && m_heights[tile + 1] != 4 )
			{
				id = Setup::eSp_High3;
			}
			MSSprite::RenderSprite( Setup::Sheet( id, m_stage ), Setup::Sprite( id, m_stage ), MSVec( i * 64 + 32 - ( m_position % 64 ), 96 ), 5, MSVec( 64, 192 ), stage.fg );
		}
	}

	// Character
	for ( int i = 0; i < NUM_CHARS; ++i )
	{
		m_char[i].Render();
	}
}

int GameImpl::ProbeHeight( int x )
{
	static const int s_levels[5] =
	{
		96 * 2,	// Floor
		90 * 2,
		65 * 2,
		50 * 2,
		22 * 2,	// Highest
	};

	const int i = ( m_position + x ) / 64;
	const int height = m_heights[i];
	assert( height >= 0 );
	assert( height < 5 );
	const int level = s_levels[height];
	return level;
}

void GameImpl::Reset()
{
	fprintf( stderr, "GameImpl reset\n" );
}

