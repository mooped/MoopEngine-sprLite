/*
 *  MSFont.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MSFONT_DEF
#define MSFONT_DEF

#include "MSEngine.h"

#include "MSRender.h"

namespace MSFont
{
	void Initialise( const char* const pCharSet, MSImage* texture, MSVec& charSize );
	void Shutdown();

	void RenderString( const char* const pszString, MSVec& pos, int layer, MSVec& size, const Colour rgba = 0xffffffff, const bool center = false );
	void RenderCharacter( const u_char character, MSVec& pos, int layer, MSVec& size, const Colour rgba = 0xffffffff );
};

#endif

