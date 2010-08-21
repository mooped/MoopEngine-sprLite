/*
 *  MSTimer.cc
 *  testgame
 *
 *  Created by Steve Barnett on 21/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MSTimer.h"

#include <ctime>
#include <sys/timeb.h>

namespace MSTimer
{
	int s_frameStart;
	int s_timeSlice;

	int GetMilliCount();
};


int MSTimer::GetMilliCount()
{
	timeb tb;
	ftime( &tb );
	return tb.millitm + (tb.time & 0xfffff) * 1000;
}

void MSTimer::Initialise()
{
	s_frameStart = GetMilliCount();
	s_timeSlice = 0;
}

void MSTimer::FrameStart()
{
	const int time = GetMilliCount();
	s_timeSlice = time - s_frameStart;
	s_frameStart = time;
}

int MSTimer::GetTime()
{
	return GetMilliCount();
}

int MSTimer::GetTimeSlice()
{
	return s_timeSlice;
}

