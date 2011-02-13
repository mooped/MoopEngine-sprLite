#ifndef GAMEIMPL_DEF
#define GAMEIMPL_DEF

#define LEVEL_LEN 128
#define NUM_CHARS 3

#include "Setup.h"

class GameImpl;

class Character
{
	Setup::EChar m_type;
	char m_key;
	MSVec m_pos;
	MSVec m_vel;
	bool m_airborne;
	int m_frame;
	GameImpl* m_pGame;
	int m_xtarget;
public:
	Character() {}
	Character( Setup::EChar type, char key, GameImpl* pGame )
	: m_type( type )
	, m_key( key )
	, m_pos( 100, 100 )
	, m_vel( 0, 0 )
	, m_airborne( false )
	, m_frame( 0 )
	, m_pGame( pGame )
	, m_xtarget( 100 )
	{
		if ( m_type == Setup::eCh_Chicken )
		{
			m_pos = MSVec( 440, 70 );
		}
	}

	void Update();
	void Render();
};

class GameImpl
{
	int m_heights[LEVEL_LEN];
	int m_backdrop[LEVEL_LEN];
	int m_position;
	int m_stage;

	bool m_victory;
	bool m_failure;

public:
	Character m_char[NUM_CHARS];

	GameImpl();
	~GameImpl();

	void Update();
	void Render();

	int ProbeHeight( int x );

	bool Victory();
	bool Failure();

	void SetFailed() { m_failure = true; }

	void Reset();
};

#endif

