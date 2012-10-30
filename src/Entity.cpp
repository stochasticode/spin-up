#include <sstream>
#include <cstdio>
#include <fstream>
#include <iostream>

#include <chipmunk.h>
#include <GL/gl.h>
#include <lua5.1/lua.hpp>
#include <tinyxml.h>

#include <Entity.h>
#include <SpinGame.h>
#include <SpinUtil.h>
#include <SpinXML.h>

using namespace spin;
using namespace std;

Entity::Entity(): dead( false ), id( -1 ), alias( "UNNAMED" ), layer( 4 )
{
	// set up lua
	lua_state = lua_open();
	luaL_openlibs( lua_state );
}

Entity::~Entity()
{
	// tear down lua
	if( lua_state != 0 )
		lua_close( lua_state );
	lua_state = 0;
}

void Entity::SetID( int new_id )
{
	id = new_id;

	// create global lua variable for entity_id
	lua_pushnumber( lua_state, id );
	lua_setglobal( lua_state, "entity_id" );
}

bool Entity::TryLoadElement( TiXmlElement* element, bool& error )
{
	error = false;

	// check for NULL
	if( element == 0 )
	{
		fprintf( stderr, "Entity::TryLoadElement -> element was NULL!\n" );
		error = true;
		return false;
	}

	// param
	if( strcmp( "param", element->Value() ) == 0 )
	{
		std::string name = "";
		std::string value = "";
		if( !ReadParam( element, name, value ) )
		{
			error = true;
			fprintf( stderr, "Entity::TryLoadElement -> ReadParam failed!\n" );
			return false;
		}

		// alias
		if( name.compare( "alias" ) == 0 )
			alias = value;
		// layer
		else if( name.compare( "layer" ) == 0 )
		{
			if( !SpinUtil::ToInt( value.c_str(), layer ) )
			{
				error = true;
				fprintf( stderr, "Entity::TryLoadElement -> invalid value for layer param!\n" );
				return false;
			}
		}
		else
			return false;
	}
	// unsupported
	else
		return false;
	
	return true;
}

bool Entity::LoadScriptString( const char* script_string, bool master )
{
	cout << "Loading script text" << endl;
	if( luaL_dostring( lua_state, script_string ) != 0 )
	{
		fprintf( stderr, "LuaEntity::LoadScript -> failed to load script string!'\n" );
		fprintf( stderr, "%s\n", lua_tostring( lua_state, -1 ) );
		
		lua_settop( lua_state, 0 );
		return false;
	}

	// save script text
	script_text = script_string;

	// save master also if appropriate
	if( master )
		master_script_text = script_text;
	return true;
}

bool Entity::LoadScript( const char* script, bool master )
{
	cout << "Loading script: " << script << endl;
	if( luaL_dofile( lua_state, script ) != 0 )
	{
		fprintf( stderr, "LuaEntity::LoadScript -> failed to load script '%s!'\n", script );
		fprintf( stderr, "%s\n", lua_tostring( lua_state, -1 ) );
		
		lua_settop( lua_state, 0 );
		return false;
	}

	// save script text
	ifstream script_file( script );
	std::stringstream buffer;
	buffer << script_file.rdbuf();
	script_text = buffer.str();

	// save master also if appropriate
	if( master )
		master_script_text = script_text;
	return true;
}

void Entity::Tick( int milliseconds )
{
	// get the lua tick function
	lua_getglobal( lua_state , "Tick" );
	if( !lua_isnil( lua_state, 1 ) )
	{
		// push first argument (milliseconds)
		lua_pushnumber( lua_state, milliseconds );
		// call tick
		lua_call( lua_state, 1, 0 );
		fflush( stdout );
	}
	else
		lua_pop( lua_state, 1 );
}
