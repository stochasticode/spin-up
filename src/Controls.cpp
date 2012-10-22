#include "Controls.h"

#include <iostream>

using namespace spin;
using namespace std;

void Controls::SetScheme( std::string scheme )
{
	key_map.clear();
	if( scheme == "player" )
	{
		// default keys
		key_map.insert( std::pair<char,ControlFunction>( 'a', CTRL_LEFT ) );
		key_map.insert( std::pair<char,ControlFunction>( 's', CTRL_DOWN ) );
		key_map.insert( std::pair<char,ControlFunction>( 'd', CTRL_RIGHT ) );
		key_map.insert( std::pair<char,ControlFunction>( 'w', CTRL_UP ) );
	
		// default mouse buttons
		mouse_button_map.insert( std::pair<int,ControlFunction>( 0, CTRL_PRIMARY ) );
		mouse_button_map.insert( std::pair<int,ControlFunction>( 1, CTRL_SECONDARY ) );
	}
	else if( scheme == "editor" )
	{
		// default keys
		key_map.insert( std::pair<char,ControlFunction>( 'h', CTRL_LEFT   ) );
		key_map.insert( std::pair<char,ControlFunction>( 'j', CTRL_DOWN   ) );
		key_map.insert( std::pair<char,ControlFunction>( 'l', CTRL_RIGHT  ) );
		key_map.insert( std::pair<char,ControlFunction>( 'k', CTRL_UP     ) );
		key_map.insert( std::pair<char,ControlFunction>( 'i', CTRL_INSERT ) );
		key_map.insert( std::pair<char,ControlFunction>( 27,  CTRL_EXIT   ) ); // esc key
		key_map.insert( std::pair<char,ControlFunction>( 8,   CTRL_LEFT   ) ); // delete key
		key_map.insert( std::pair<char,ControlFunction>( 'x', CTRL_DELETE ) );
		key_map.insert( std::pair<char,ControlFunction>( 13,  CTRL_ACCEPT ) ); // enter
	}
	else
		cerr << "Controls::SetScheme -> invalid scheme: " << scheme << "!\n";
}

void Controls::MouseDown( int button )
{
}

void Controls::MouseUp( int button )
{
}

void Controls::KeyDown( char key )
{
	cout << "key: " << (int)key << endl;

	// process raw key event
	ProcessRawKeyEvent( RawKeyEvent( key, true ) );

	// process control event
	ControlFunction function = GetFunction( key );
	if( function != CTRL_UNDEFINED)
		ProcessControlEvent( ControlEvent( function, true ) );
}

void Controls::KeyUp( char key )
{
	// process raw key event
	ProcessRawKeyEvent( RawKeyEvent( key, false ) );

	// process control event
	ControlFunction function = GetFunction( key );
	if( function != CTRL_UNDEFINED)
		ProcessControlEvent( ControlEvent( function, false ) );
}

ControlFunction Controls::GetFunction( char key )
{
	std::map<char,ControlFunction>::iterator it = key_map.find( key );
	return (it == key_map.end()) ? CTRL_UNDEFINED : it->second;
}
