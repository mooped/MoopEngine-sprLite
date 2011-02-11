/*
 *  MSLauncher_Backend.h
 *  sokoban_ios
 *
 *  Created by Steve Barnett on 04/10/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MSLAUNCHER_BACKEND_DEF
#define MSLAUNCHER_BACKEND_DEF

namespace MSLauncher
{
	void ReshapeCallback( int width, int height );
	void DisplayCallback();
	void ExitCallback();

	void KeyDownCallback( unsigned char key, int x, int y );
	void KeyUpCallback( unsigned char key, int x, int y );
	void KeySpecialCallback( int key, int x, int y );

	void TickRT();
};

#endif

