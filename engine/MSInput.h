/*
 *  MSInput.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MSINPUT_DEF
#define MSINPUT_DEF

namespace MSInput
{
	void Initialise();
	void Shutdown();

	bool Key( char key );
	bool DebouncedKey( char key );
	void ResetKey( char key );

	void ResetKeys();
	void ResetFrameKeys();
};

#endif

