/*
 *  MSLauncher.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MSGLUT_DEF
#define MSGLUT_DEF

#include "MSEngine.h"

namespace MSMain
{
	void Main();
};

namespace MSLauncher
{
	typedef void (TickFn)();
	typedef void (Callback)();
	typedef void (KeyCallback)(unsigned char key);

	const char* GetBasePath();

	void RequestRedisplay();
	void SwapBuffers();

	void SetDisplayCallback( Callback* pfnCallback );
	void SetExitCallback( Callback* pfnCallback );
	void SetKeyUpCallback( KeyCallback* pfnCallback );
	void SetKeyDownCallback( KeyCallback* pfnCallback );

	void Launch( TickFn* pfnTick );

	void SetTitle( const char* const title );
};

#endif

