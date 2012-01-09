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
	lua_register( lua_state, "QuadEntitySetColorAll", lua_QuadEntitySetColorAll ); 
	lua_register( lua_state, "GetEntityID", lua_GetEntityID ); 
	lua_register( lua_state, "SetCameraMode", lua_SetCameraMode ); 
	lua_register( lua_state, "SetCameraPosition", lua_SetCameraPosition ); 
	lua_register( lua_state, "SetCameraZoom", lua_SetCameraZoom ); 
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
	InitScript();
	return true;
}

void LuaEntity::InitScript()
{
	// get the init function
	lua_getglobal( lua_state , "Init" );
	if( lua_gettop( lua_state ) != 1 || lua_isnil( lua_state, 1 ) )
	{
		fprintf( stderr, "LuaEntity::InitLua-> couldn't get global variable 'init!'\n" );
		return;
	}
	// call init 
	lua_call( lua_state, 0, 0 );
	//fflush( stdout );
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

int LuaEntity::lua_QuadEntitySetColorAll( lua_State *L )
{
	if( lua_gettop( L ) < 5 )
	{
		fprintf( stderr, "LuaEntity::lua_QuadEntityColorAll-> not enough lua arguments!\n" );
		return 0;
	}

	// get entity_id
	int quad_id = lua_tonumber( L, 1 );
	Color new_color;
	new_color.r = lua_tonumber( L, 2 );
	new_color.g = lua_tonumber( L, 3 );
	new_color.b = lua_tonumber( L, 4 );
	new_color.a = lua_tonumber( L, 5 );

	Entity* entity = SPIN.world.GetEntity( quad_id );
	QuadEntity* quad_entity = 0;
	if( entity == 0 || (quad_entity = dynamic_cast<QuadEntity*>(entity) ) == 0 )
	{
		fprintf( stderr, "LuaEntity::lua_QuadEntitySetColorAll -> no QuadEntity found for entity id: %d!\n", quad_id );
		return 0;
	}
	quad_entity->SetColorAll( new_color );

	return 0;
}

int LuaEntity::lua_SetCameraPosition( lua_State *L )
{
	if( lua_gettop( L ) < 2 )
	{
		fprintf( stderr, "LuaEntity::lua_SetCameraPosition -> not enough lua arguments!\n" );
		return 0;
	}

	// get camera position
	Vector new_position;
	new_position.x = lua_tonumber( L, 1 );
	new_position.y = lua_tonumber( L, 2 );

	SPIN.camera.position_x = new_position.x;
	SPIN.camera.position_y = new_position.y;

	return 0;
}

int LuaEntity::lua_SetCameraZoom( lua_State *L )
{
	if( lua_gettop( L ) < 1 )
	{
		fprintf( stderr, "LuaEntity::lua_SetCameraZoom -> not enough lua arguments!\n" );
		return 0;
	}

	// get camera zoom
	float zoom = lua_tonumber( L, 1 );
	if( zoom <= 0 )
	{
		fprintf( stderr, "LuaEntity::lua_SetCameraZoom -> invalid zoom %f!\n", zoom );
		return 0;
	}

	SPIN.camera.zoom = zoom;

	return 0;
}

int LuaEntity::lua_GetEntityID( lua_State *L )
{
	if( lua_gettop( L ) < 1 )
	{
		fprintf( stderr, "LuaEntity::lua_GetEntityID -> not enough lua arguments!\n" );
		lua_pushnumber( L, 0 );
		return 1;
	}

	// get alias
	const char* alias = lua_tostring( L, 1 );
	if( alias == 0 )
	{
		fprintf( stderr, "LuaEntity::lua_GetEntityID -> first argument wasn't a string!\n" );
		lua_pushnumber( L, 0 );
		return 1;
	}

	//printf( "quad_id: %d, x: %f, y: %f\n", quad_id, new_position.x, new_position.y );
	//fflush( stdout );

	std::string alias_string = alias;
	Entity* entity = SPIN.world.GetEntityByAlias( alias_string );
	if( entity == 0  )
	{
		fprintf( stderr, "LuaEntity::lua_GetEntityID -> no Entity found for entity alias: %s!\n",  alias );
		lua_pushnumber( L, 0 );
		return 1;
	}

	lua_pushnumber( L, entity->GetID() );
	return 1;
}

int LuaEntity::lua_SetCameraMode( lua_State *L )
{
	if( lua_gettop( L ) < 1 )
	{
		fprintf( stderr, "LuaEntity::lua_SetCameraMode -> not enough lua arguments!\n" );
		return 0;
	}

	// get alias
	const char* mode = lua_tostring( L, 1 );
	if( mode == 0 )
	{
		fprintf( stderr, "LuaEntity::lua_SetCameraMode -> first argument wasn't a string!\n" );
		return 0;
	}

	if( strcmp( "stationary", mode ) == 0 )
		SPIN.camera_mode = SpinGame::CAMERA_STATIONARY;
	else if( strcmp( "following", mode ) == 0 )
		SPIN.camera_mode = SpinGame::CAMERA_FOLLOWING;
	else
	{
		fprintf( stderr, "LuaEntity::lua_SetCameraMode -> invalid camera mode '%s!'\n", mode );
		lua_pushnumber( L, 0 );
		return 0;
	}

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
