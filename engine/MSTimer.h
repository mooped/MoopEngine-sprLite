/*
 *  MSTimer.h
 *  testgame
 *
 *  Created by Steve Barnett on 21/08/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MSTIMER_DEF
#define MSTIMER_DEF

#include "MSEngine.h"

namespace MSTimer
{
	void Initialise();
	void FrameStart();

	int GetTime();
	int GetTimeSlice();
};

#endif

