#ifndef GAMEIMPL_DEF
#define GAMEIMPL_DEF

#define LEVEL_LEN 64
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
	{}

	void Update();
	void Render();
};

class GameImpl
{
	int m_heights[LEVEL_LEN];
	int m_backdrop[LEVEL_LEN];
	int m_position;
	int m_stage;

public:
	Character m_char[NUM_CHARS];

	GameImpl();
	~GameImpl();

	void Update();
	void Render();

	int ProbeHeight( int x );

	void Reset();
};

#endif

