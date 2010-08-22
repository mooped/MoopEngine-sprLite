// Engine
#include "MSEngine.h"
#include "MSRenderThread.h"
#include "MSInput.h"

// Launcher
#include "MSLauncher.h"

// System
#include <stdio.h>

#define MULTITHREAD 0

#if MULTITHREAD
#include "GameThread.h"
static MSThread* gt;
#else
#include "MSCmdBuf.h"
#endif

// Game
#include "Game.h"

static Game* game;

void ExitCallback()
{
#if MULTITHREAD
	delete gt;
#endif
	MSInput::Shutdown();
}

void UpdateGame()
{
	if ( game )
	{
		game->Update();
	}
}

void MSMain::Main()
{
	MSLauncher::SetTitle( "sprLite Window" );
	MSLauncher::SetExitCallback( &ExitCallback );
	MSInput::Initialise();

#if MULTITHREAD
	// Launch game thread
	printf( "[Main] Launching game thread\n" );
	gt = new GameThread;
	gt->Run();
#else
	printf( "[Main] Launching game\n" );
	MSCmdBuf::SetMultithread( false );
	MSRenderThread::SetGameUpdateFunc( &UpdateGame );
	game = new Game;
	game->Begin();
#endif

	// Launch render thread
	printf( "[Main] Launching render thread\n" );
	MSRenderThread::CreateRT();
}

