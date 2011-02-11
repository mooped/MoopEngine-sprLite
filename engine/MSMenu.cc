/*
 *  MSMenu.cc
 *  sokoban
 *
 *  Created by Steve Barnett on 11/10/2010.
 *  Copyright 2010 Stephen Barnett. All rights reserved.
 *
 */

#include "MSMenu.h"

#include "MSEngine.h"
#include "MSPoint.h"
#include "MSFont.h"
#include "MSInput.h"
#include "MSTimer.h"

#include <string.h>

namespace MSMenu_Int
{
	const MSVec fontSize = MSVec( 16, 16 );

	class Item
	{
	public:
		virtual ~Item() {};
		virtual MSMenu::EAction Update( bool selected ) = 0;
		virtual void Render( MSVec& pos, bool selected ) = 0;
		virtual MSVec GetSize() = 0;
	};

	class Spacer : public Item
	{
		virtual MSMenu::EAction Update( bool selected )
		{
			// Nothing to see here
			return MSMenu::eAt_Continue;
		}

		virtual void Render( MSVec& pos, bool selected )
		{
			// Nothing here either
		}

		virtual MSVec GetSize()
		{
			return fontSize;
		}
	};

	class Label : public Item
	{
	protected:
		char* const m_label;
	public:
		Label( const char* const label )
		: m_label( new char[strlen( label ) + 1] )
		{
			strcpy( m_label, label );
		}

		virtual ~Label()
		{
			delete[] m_label;
		}

		virtual MSMenu::EAction Update( bool selected )
		{
			// Nothing to see here
			return MSMenu::eAt_Continue;
		}

		virtual void Render( MSVec& pos, bool selected )
		{
			MSFont::RenderString( m_label, pos, 1, fontSize, 0xffffffff );
		}

		virtual MSVec GetSize()
		{
			return MSFont::CalculateSize( m_label, fontSize );
		}
	};

	class Button : public Label
	{
	protected:
		MSMenu::Callback* m_callback;
		void *m_data;

	public:
		Button( const char* const label, MSMenu::Callback* callback, void* data )
		: Label( label )
		, m_callback( callback )
		, m_data( data )
		{}

		virtual ~Button() {};

		virtual MSMenu::EAction Update( bool selected )
		{
			if ( m_callback && selected )
			{
				if ( MSInput::Key( ' ' ) || MSInput::Key( '\r' ) || MSInput::Key( '\n' ) )
				{
					MSInput::ResetKey( ' ' );
					MSInput::ResetKey( '\r' );
					MSInput::ResetKey( '\n' );
					m_callback( m_data );
				}
			}
			return MSMenu::eAt_None;
		}

		virtual void Render( MSVec& pos, bool selected )
		{
			u_int highlight = selected ? 0xff2f2fff : 0x7f2f2fff;
			MSFont::RenderString( m_label, pos, 1, fontSize, highlight );
		}

		using Label::GetSize;
	};

	class Boolean : public Label
	{
	protected:
		bool& m_data;

	public:
		Boolean( const char* const label, bool& data )
		: Label( label )
		, m_data( data )
		{}

		virtual ~Boolean() {};

		virtual MSMenu::EAction Update( bool selected )
		{
			if ( selected )
			{
				if ( MSInput::Key( 'a' ) || MSInput::Key( 'd' ) )
				{
					MSInput::ResetKey( 'a' );
					MSInput::ResetKey( 'd' );
					m_data = m_data ? false : true;
				}
			}
			return MSMenu::eAt_None;
		}

		virtual void Render( MSVec& pos, bool selected )
		{
			Label::Render( pos, selected );
			MSVec szLabel = Label::GetSize();
			MSVec yesSize = MSFont::CalculateSize( "YES", fontSize );
			szLabel.y = 0;
			yesSize.y = 0;

			u_int highlighta = selected ? 0xff2f2fff : 0x7f2f2fff;
			u_int highlightb = selected ? 0xffffffff : 0x7f7f7fff;

			MSFont::RenderString( "YES", pos + szLabel + MSVec( fontSize.x * 2, 0 ), 1, fontSize, m_data ? highlighta : highlightb );
			MSFont::RenderString( "NO", pos + szLabel + MSVec( fontSize.x * 4, 0 ) + yesSize, 1, fontSize, m_data ? highlightb : highlighta );
		}

		virtual MSVec GetSize()
		{
			return Label::GetSize() + MSVec( MSFont::CalculateSize( "YES", fontSize ).x + MSFont::CalculateSize( "NO", fontSize ).x + fontSize.x * 2, 0 );
		}
	};

	class TextArea : public Label
	{
	protected:
		char* const m_data;
		int m_len;

		int m_nextCursorUpdate;
		bool m_cursorState;

		bool m_active;

	public:
		TextArea( const char* const label, char* const data, int maxlen )
		: Label( label )
		, m_data( data )
		, m_len( maxlen )
		, m_nextCursorUpdate( 0 )
		, m_cursorState( false )
		, m_active( false )
		{
			MASSERT( data && ( strlen( data ) < maxlen ), "Invalid string in textfield" );
		}

		virtual ~TextArea() {};

		virtual MSMenu::EAction Update( bool selected )
		{
			if ( selected )
			{
				if ( m_active )
				{
					const char* const charset = MSFont::GetCharSet();
					const int len = strlen( m_data );
					if ( ( len < m_len - 1 ) && ( MSInput::Key( '\10' ) || MSInput::Key( '\177' ) ) )
					{
						MSInput::ResetKey( '\10' );
						MSInput::ResetKey( '\177' );
						if ( len > 0 ) { m_data[len - 1] = '\0'; }
					}
					else if ( MSInput::Key( '\n' ) || MSInput::Key( '\r' ) )
					{
						MSInput::ResetKey( '\n' );
						MSInput::ResetKey( '\r' );
						return MSMenu::eAt_Next;
					}
					else if ( charset && m_len > 0 && len < m_len - 1 )
					{
						const int cslen = strlen( charset );
						for ( int i = 0; i < cslen; ++i )
						{
							if ( MSInput::Key( charset[i] ) )
							{
								MSInput::ResetKey( charset[i] );
								m_data[len] = charset[i];
								m_data[len + 1] = '\0';
							}
						}
						if ( MSInput::Key( ' ' ) )
						{
							MSInput::ResetKey( ' ' );
							m_data[len] = ' ';
							m_data[len + 1] = '\0';
						}
					}
				}
				else if ( MSInput::Key( '\n' ) || MSInput::Key( '\r' ) || MSInput::Key( ' ' ) )
				{
					MSInput::ResetKey( '\n' );
					MSInput::ResetKey( '\r' );
					MSInput::ResetKey( ' ' );
					m_active = true;
				}
			}
			else
			{
				m_active = false;
			}
			return MSMenu::eAt_None;
		}

		virtual void Render( MSVec& pos, bool selected )
		{
			bool cursorState = true;
			if ( selected )
			{
				const int time = MSTimer::GetTime();
				if ( time > m_nextCursorUpdate )
				{
					m_nextCursorUpdate = time + 250;
					m_cursorState = !m_cursorState;
				}
				cursorState = m_cursorState;
			}
			Label::Render( pos, selected );
			MSFont::RenderString( m_data, pos + MSVec( Label::GetSize().x + fontSize.x * 2, 0 ), 1, fontSize, m_active ? 0xffffffff : 0x7f7f7fff );
			if ( cursorState )
			{
				MSFont::RenderString( ".", pos + MSVec( GetSize().x, 0 ), 1, fontSize, selected ? 0xff2f2fff : 0xffffffff );
			}
		}

		virtual MSVec GetSize()
		{
			return Label::GetSize() + MSVec( MSFont::CalculateSize( m_data, fontSize ).x + fontSize.x * 3, 0 );
		}
	};
};

using namespace MSMenu_Int;

void MSMenu::ProcessSelectionAction( const MSMenu::EAction action )
{
	bool store = false;
	switch (action )
	{
		case eAt_Next:
		{
			++m_selected;
			store = true;
		} break;
		case eAt_Prev:
		{
			if ( m_selected == 0 )
			{
				m_selected = m_items.size() - 1;
			}
			else
			{
				--m_selected;
			}
			store = true;
		} break;
		case eAt_Continue:
		{
			ProcessSelectionAction( m_lastAction );
		} break;
		case eAt_None: break;
		default: MASSERT( 0, "Unexpected menu action!" ); break;
	};
	if ( store )
	{
		m_lastAction = action;
	}
}

MSMenu::MSMenu()
: m_selected( 0 )
, m_lastAction( eAt_Next )
{

}

MSMenu::~MSMenu()
{
	ItemList::iterator end = m_items.end();
	for ( ItemList::iterator it = m_items.begin(); it != end; ++it )
	{
		delete *it;
	}
	m_items.clear();
}

void MSMenu::RegisterField( const char* const label )
{
	m_items.push_back( new Label( label ) );
}

void MSMenu::RegisterField( const char* const label, Callback* callback, void* data )
{
	m_items.push_back( new Button( label, callback, data ) );
}

//void MSMenu::RegisterField( const char* const label, int& f, const int min, const int max )

//void MSMenu::RegisterField( const char* const label, int& f, const int nvals, const char* const vals )

void MSMenu::RegisterField( const char* const label, bool& f )
{
	m_items.push_back( new Boolean( label, f ) );
}

void MSMenu::RegisterField( const char* const label, char* const f, const int maxlen )
{
	m_items.push_back( new TextArea( label, f, maxlen ) );
}

void MSMenu::RegisterSpacer()
{
	m_items.push_back( new Spacer() );
}

void MSMenu::Update()
{
	if ( m_selected >= m_items.size() )
	{
		m_selected = 0;
	}
	
	ItemList::iterator begin = m_items.begin();
	ItemList::iterator end = m_items.end();
	for ( ItemList::iterator it = begin; it != end; ++it )
	{
		if ( std::distance( begin, it ) == m_selected )
		{
			ProcessSelectionAction( (*it)->Update( true ) );
		}
		else
		{
			(*it)->Update( false );
		}
	}

	if ( MSInput::Key( 'w' ) )
	{
		MSInput::ResetKey( 'w' );
		ProcessSelectionAction( eAt_Prev );
	}
	else if ( MSInput::Key( 's' ) )
	{
		MSInput::ResetKey( 's' );
		ProcessSelectionAction( eAt_Next );
	}

}

void MSMenu::Render( const MSVec& pos )
{
	ItemList::iterator begin = m_items.begin();
	ItemList::iterator end = m_items.end();
	MSVec cpos = pos;
	for ( ItemList::iterator it = begin; it != end; ++it )
	{
		const bool sel = ( std::distance( begin, it ) == m_selected );
		(*it)->Render( cpos, sel );

		cpos.y += (*it)->GetSize().y;
	}
}

