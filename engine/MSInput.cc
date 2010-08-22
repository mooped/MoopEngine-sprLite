/*
 *  MSInput.cc
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MSInput.h"

#include "MSLauncher.h"

#define NKEYS 256

namespace MSInput
{
	bool s_keyState[NKEYS];
	bool s_keyPressed[NKEYS];

	void KeyUp( unsigned char key );
	void KeyDown( unsigned char key );
};

// Internals
void MSInput::KeyUp( unsigned char key )
{
	s_keyState[key] = false;
}

void MSInput::KeyDown( unsigned char key )
{
	s_keyState[key] = true;
	s_keyPressed[key] = true;
}

// Interface implementation
void MSInput::Initialise()
{
	ResetKeys();
	MSLauncher::SetKeyUpCallback( &KeyUp );
	MSLauncher::SetKeyDownCallback( &KeyDown );
}

void MSInput::Shutdown()
{
	ResetKeys();
	MSLauncher::SetKeyUpCallback( NULL );
	MSLauncher::SetKeyDownCallback( NULL );
}

bool MSInput::Key( char key )
{
	return s_keyState[key];
}

bool MSInput::DebouncedKey( char key )
{
	return s_keyPressed[key];
}

void MSInput::ResetKey( char key )
{
	s_keyState[key] = false;
	s_keyPressed[key] = false;
}

void MSInput::ResetKeys()
{
	for ( int i = 0; i < NKEYS; ++i )
	{
		s_keyState[i] = false;
		s_keyPressed[i] = false;
	}
}

void MSInput::ResetFrameKeys()
{
	for ( int i = 0; i < NKEYS; ++i )
	{
		s_keyPressed[i] = false;
	}
}

