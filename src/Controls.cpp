#include "Controls.h"

using namespace spin;

Controls::Controls()
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

void Controls::KeyDown( char key )
{
	// find binding
	std::map<char,ControlFunction>::iterator it = key_map.find( key );

}
