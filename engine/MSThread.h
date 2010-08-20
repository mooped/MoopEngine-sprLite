/*
 *  MSThread.h
 *  testgame
 *
 *  Created by Steve Barnett on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MTHREAD_DEF
#define MTHREAD_DEF

#include "MSEngine.h"

class MSThread
{
public:
	MSThread( void );
	virtual ~MSThread( void );

	void Run( void );
protected:
	virtual void RunThread( void );	// Override this. Default just exits.
	inline bool ExitRequested( void ) { return m_kill; }
	void Exit( void );
private:

	static void* RunThread_Impl( void* pInst );
	static void RunThreadNoRV_Impl( void* pInst );

	struct ThreadInstance* m_data;
	bool m_kill;
	bool m_dead;
};

#endif

