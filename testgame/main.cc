// Engine
#include "MSEngine.h"
#include "MSRenderThread.h"
#include "MSInput.h"

// Launcher
#include "MSLauncher.h"

// System
#include <stdio.h>

// Game
#include "GameThread.h"

#define MULTITHREAD 0

static MSThread* gt;
static Game* game;

void ExitCallback()
{
	delete gt;
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
	game = new Game;
	game->Begin();
	MSRenderThread::SetGameUpdateFunc( &UpdateGame );
#endif

	// Launch render thread
	printf( "[Main] Launching render thread\n" );
	MSRenderThread::CreateRT();
}

