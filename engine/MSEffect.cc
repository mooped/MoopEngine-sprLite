/*
 *  MSEffect.cc
 *  sokoban_ios
 *
 *  Created by Steve Barnett on 06/10/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#include "MSEffect.h"

void MSEffect::Sprite( SEffectParams& p )
{
	p.type = ET_Sprite;
	p.nFloats = 0;
	p.auxComponents = 0;
	p.activeStreams = SM_POSITION | SM_COLOUR | SM_TEXCOORD;
}

void MSEffect::Glow( SEffectParams& p, float inner, float outer, unsigned int rgba)
{
	p.type = ET_Glow;

	p.fParams[0] = inner;
	p.fParams[1] = outer;
	p.fParams[2] = 0.f;
	p.fParams[3] = 0.f;
	const unsigned char ucRed = ( (rgba & 0xff000000) >> 24 );
	const unsigned char ucGreen = ( (rgba & 0x00ff0000) >> 16 );
	const unsigned char ucBlue = ( (rgba & 0x0000ff00) >> 8 );
	const unsigned char ucAlpha = ( rgba & 0x000000ff );
	p.fParams[4] = ucRed / 255.f;
	p.fParams[5] = ucGreen / 255.f;
	p.fParams[6] = ucBlue / 255.f;
	p.fParams[7] = ucAlpha / 255.f;

	p.nFloats = 8;
	p.auxComponents = 2;
	p.activeStreams = SM_POSITION | SM_TEXCOORD | SM_AUX;
}

