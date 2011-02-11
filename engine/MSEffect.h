/*
 *  MSEffect.h
 *  sokoban_ios
 *
 *  Created by Steve Barnett on 06/10/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MSEFFECT_DEF
#define MSEFFECT_DEF

#include "MSEffectTypes.h"

namespace MSEffect
{
	enum SStreamMask
	{
		SM_NONE = 0x00,
		SM_POSITION = 0x01,
		SM_COLOUR = 0x02,
		SM_TEXCOORD = 0x04,
		SM_AUX = 0x08,
		SM_ALL = ~0x00,
	};

	struct SEffectParams
	{
		EEffectType type;
		float fParams[8];
		unsigned int nFloats;
		unsigned auxComponents;
		unsigned int activeStreams;
	};

	void Sprite( SEffectParams& p );
	void Glow( SEffectParams& p, float inner, float outer, unsigned int rgba );
};

#endif

