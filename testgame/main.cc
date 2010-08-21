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

static MSThread* gt;

void ExitCallback()
{
	delete gt;
	MSInput::Shutdown();
}

void MSMain::Main()
{
	MSLauncher::SetTitle( "sprLite Window" );
	MSLauncher::SetExitCallback( &ExitCallback );
	MSInput::Initialise();

	// Launch game thread
	printf( "[Main] Launching game thread\n" );
	gt = new GameThread;
	gt->Run();

	// Launch render thread
	printf( "[Main] Launching render thread\n" );
	MSRenderThread::CreateRT();
}

