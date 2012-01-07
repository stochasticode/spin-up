#include <LuaEntity.h>
#include <SpinGame.h>
#include <tinyxml.h>
#include <lua5.1/lua.hpp>
#include <cstdlib>

using namespace spin;

LuaEntity::LuaEntity()
{
	lua_state = lua_open();
	luaL_openlibs( lua_state );

	// register functions
	lua_register( lua_state, "KillEntity", lua_KillEntity ); 
	lua_register( lua_state, "LoadEntity", lua_LoadEntity ); 
	lua_register( lua_state, "QuadEntitySetPosition", lua_QuadEntitySetPosition ); 
}

LuaEntity::~LuaEntity()
{
	if( lua_state != 0 )
		lua_close( lua_state );
	lua_state = 0;
}

void LuaEntity::SetID( int new_id )
{
	Entity::SetID( new_id );

	// create global for entity_id
	lua_pushnumber( lua_state, id );
	lua_setglobal( lua_state, "entity_id" );
}

bool LuaEntity::LoadScript( const char* script )
{
	if( luaL_dofile( lua_state, script ) != 0 )
	{
		fprintf( stderr, "LuaEntity::LoadScript -> failed to load script '%s!'\n", script );
		return false;
	}
	return true;
}

void LuaEntity::Tick( int milliseconds )
{
	// get the tick function
	lua_getglobal( lua_state , "Tick" );
	if( lua_gettop( lua_state ) != 1 || lua_isnil( lua_state, 1 ) )
	{
		fprintf( stderr, "LuaEntity::Tick -> couldn't get global variable 'tick!'\n" );
		return;
	}
	// push first argument (milliseconds)
	lua_pushnumber( lua_state, milliseconds );
	// call tick
	lua_call( lua_state, 1, 0 );

	fflush( stdout );
}

int LuaEntity::lua_LoadEntity( lua_State *L )
{
	if( lua_gettop( L ) < 1 )
	{
		fprintf( stderr, "LuaEntity::lua_LoadEntity -> not enough lua arguments!\n" );
		return 0;
	}

	// get xml_path
	const char* xml_path = lua_tostring( L, 1 );
	if( xml_path == 0 )
	{
		fprintf( stderr, "LuaEntity::lua_LoadEntity -> not enough lua arguments!\n" );
		return 0;
	}

	lua_pushnumber( L, SPIN.world.LoadEntity( xml_path ) );
	return 1;
}

int LuaEntity::lua_KillEntity( lua_State *L )
{
	if( lua_gettop( L ) < 1 )
	{
		fprintf( stderr, "LuaEntity::lua_KillEntity -> not enough lua arguments!\n" );
		return 0;
	}

	// get entity_id
	int entity_id = lua_tonumber( L, 1 );

	Entity* doomed = SPIN.world.GetEntity( entity_id );
	if( doomed != 0 )
		doomed->dead = true;

	return 0;
}

int LuaEntity::lua_QuadEntitySetPosition( lua_State *L )
{
	if( lua_gettop( L ) < 3 )
	{
		fprintf( stderr, "LuaEntity::lua_QuadEntitySetPosition -> not enough lua arguments!\n" );
		return 0;
	}

	// get entity_id
	int quad_id = lua_tonumber( L, 1 );
	Vector new_position;
	new_position.x = lua_tonumber( L, 2 );
	new_position.y = lua_tonumber( L, 3 );

	printf( "quad_id: %d, x: %f, y: %f\n", quad_id, new_position.x, new_position.y );
	fflush( stdout );

	Entity* entity = SPIN.world.GetEntity( quad_id );
	QuadEntity* quad_entity = 0;
	if( entity == 0 || (quad_entity = dynamic_cast<QuadEntity*>(entity) ) == 0 )
	{
		fprintf( stderr, "LuaEntity::lua_LoadEntity -> no QuadEntity found for entity id: %d!\n", quad_id );
		return 0;
	}
	quad_entity->SetPosition( new_position );

	return 0;
}

bool LuaEntity::TryLoadElement( TiXmlElement* element, bool& error )
{
	error = false;

	// check for NULL
	if( element == 0 )
	{
		fprintf( stderr, "LuaEntity::TryLoadElement -> element was NULL!\n" );
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
			fprintf( stderr, "LuaEntity::TryLoadElement -> ReadParam failed!\n" );
			return false;
		}

		// script
		if( name.compare( "script" ) == 0 )
		{
			if( !LoadScript( value.c_str() ) )
			{
				error = true;
				fprintf( stderr, "LuaEntity::TryLoadElement -> LoadScript failed for script '%s!'\n", value.c_str() );
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
