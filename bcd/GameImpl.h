#ifndef GAMEIMPL_DEF
#define GAMEIMPL_DEF

#define LEVEL_LEN 64
#define NUM_CHARS 3

#include "Setup.h"

class Character
{
	Setup::EChar m_type;
	MSVec m_pos;
	MSVec m_vel;
	bool m_airborne;
	int m_frame;
public:
	Character() {}
	Character( Setup::EChar type )
	: m_type( type )
	, m_pos( 100, 100 )
	, m_vel( 0, 0 )
	, m_airborne( false )
	, m_frame( 0 )
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

	Character m_char[NUM_CHARS];

public:
	GameImpl();
	~GameImpl();

	void Update();
	void Render();

	void Reset();
};

#endif

