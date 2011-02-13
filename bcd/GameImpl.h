#ifndef GAMEIMPL_DEF
#define GAMEIMPL_DEF

#define LEVEL_LEN 64

class GameImpl
{
	int m_heights[LEVEL_LEN];
	int m_backdrop[LEVEL_LEN];
	int m_position;

public:
	GameImpl();
	~GameImpl();

	void Update();
	void Render();

	void Reset();
};

#endif

