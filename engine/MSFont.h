/*
 *  MSFont.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MSFONT_DEF
#define MSFONT_DEF

#include "MSEngine.h"

#include "MSImage.h"
#include "MSPoint.h"

namespace MSFont
{
	void Initialise( const char* const pCharSet, MSImage* texture, const MSVec& charSize );
	void Shutdown();

	void RenderString( const char* const pszString, const MSVec& pos, int layer, const MSVec& size, const Colour rgba = 0xffffffff, const bool center = false );
	void RenderCharacter( const u_char character, const MSVec& pos, int layer, const MSVec& size, const Colour rgba = 0xffffffff );

	MSVec CalculateSize( const char* const pszString, const MSVec& size );
};

#endif

