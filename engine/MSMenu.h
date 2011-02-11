/*
 *  MSMenu.h
 *  sokoban
 *
 *  Created by Steve Barnett on 11/10/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#ifndef MSMENU_DEF
#define MSMENU_DEF

#include "MSPoint.h"
#include <vector>

namespace MSMenu_Int
{
	class Item;
};

class MSMenu
{
	typedef std::vector<MSMenu_Int::Item*> ItemList;

public:
	enum EAction
	{
		eAt_None = 0,
		eAt_Next,
		eAt_Prev,
		eAt_Continue,
	};

	typedef void (Callback)(void*);

	MSMenu();
	virtual ~MSMenu();

	void RegisterField( const char* const label );
	void RegisterField( const char* const label, Callback* callback, void* data );
	//void RegisterField( const char* const label, int& f, const int min, const int max );
	//void RegisterField( const char* const label, int& f, const int nvals, const char* const vals );
	void RegisterField( const char* const label, bool& f );
	void RegisterField( const char* const label, char* const f, const int maxlen );
	void RegisterSpacer();

	void Update();
	void Render( const MSVec& pos );

private:
	ItemList m_items;
	unsigned int m_selected;
	EAction m_lastAction;

	void ProcessSelectionAction( const EAction action );
};

#endif

