#include "MSEngine.h"
#include "MSLauncher.h"

#include "MSRenderThread.h"

#include <stdio.h>

void MSMain::Main()
{
	MSLauncher::SetTitle( "sprLite Window" );
	// Launch game thread
	printf( "[Main] Launching game thread\n" );

	// Launch render thread
	printf( "[Main] Launching render thread\n" );
	MSRenderThread::CreateRT();
}

