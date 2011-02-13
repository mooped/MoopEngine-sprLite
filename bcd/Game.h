/*
 *  Game.h
 *  bunny chicken dinosaur
 *
 *  Created by Steve Barnett on 12/02/2011.
 *  Copyright 2011 Stephen Barnett. All rights reserved.
 *
 */

#ifndef GAME_DEF
#define GAME_DEF

#include "MSEngine.h"

class MSMenu;
class GameImpl;
struct STitleMenu;
struct SGameMenu;

enum { GAME_VERSION = 1 };

class Game
{
private:
	enum EState
	{
		eS_Title = 0,
		eS_Game,
		eS_GameMenu,
		eS_Victory,
		eS_Failure,
		eS_Count,
	};

	bool m_quit;

	int m_nextTick;
	EState m_state;
	EState m_pauseState;

	MSMenu* m_pMenu;
	STitleMenu* m_pTitleMenu;
	SGameMenu* m_pGameMenu;

	GameImpl* m_pGame;

protected:
	void UpdateTitle();
	void UpdateGame();
	void UpdateGameMenu();
	void UpdateVictory();
	void UpdateFailure();

public:
	Game();
	~Game();

	void Update();
	bool ShouldQuit() { return m_quit; }
};

#endif

