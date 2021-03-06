#include <World.h>
#include <SpinGame.h>
#include <Entity.h>
#include <TextEntity.h>
#include <ConstraintEntity.h>
#include <SnapConstraint.h>
#include <LuaEntity.h>
#include <SpinXML.h>
#include <chipmunk.h>
#include <tinyxml.h>
#include <cstdio>
#include <sstream>

using namespace spin;

World::World(): space( cpSpaceNew() ), static_body( 0 ), delta_tick( 100.0 / 80.0 ), delta_tick_seconds( delta_tick / 1000.0 ), last_tick( -1 )
{
	// set up space
	cpSpaceSetGravity( space, cpv( 0, -130 ) ); 
	space->enableContactGraph = cpTrue;

	//cpSpaceSetIterations( space, 30 );
	//cpSpaceSetDamping( space, 0.75 );

	//cpSpaceAddCollisionHandler( space, World::COL_TYPE_GRAPPLE, World::COL_TYPE_SURFACE, 0, 0, GrappleGun::PostSolveGrapple, 0, 0);
	//cpSpaceAddCollisionHandler( space, World::COL_TYPE_GRAPPLE, World::COL_TYPE_PROP, 0, 0, GrappleGun::PostSolveGrapple, 0, 0);
}

World::~World()
{
	// delete constraints first...
	for( int i = 0; i < entities.size(); i++ )
	{
		if( entities[i] != 0 && dynamic_cast<ConstraintEntity*>(entities[i]) != 0 )
		{
			delete entities[i];
			entities[i] = 0;
		}
	}
	// delete entities
	for( int i = 0; i < entities.size(); i++ )
	{
		if( entities[i] != 0 )
		{
			delete entities[i];
			entities[i] = 0;
		}
	}
	// delete space
	if( space != 0 )
		cpSpaceFree( space );
}

void World::Init()
{
	// set up static body
	static_body = new StaticBody();
	static_body->alias = "STATIC";
	AddEntity( static_body, 1 );
}

void World::Render()
{
	glPushMatrix();

	// apply camera
	SPIN.camera.Apply();

	// render entities
	for( int i = 0; i < SPIN_ENTITY_LAYERS; i++ )
		for( int j = 0; j < layers[i].size(); j++ )
			layers[i][j]->Render();

	glPopMatrix();
}

bool World::Tick( int milliseconds )
{
	if( last_tick < 0 )
	{
		last_tick = milliseconds;
		return false;
	}


	int new_delta_tick = milliseconds - last_tick;
	int ticks_to_run = new_delta_tick / delta_tick;
	if( ticks_to_run > 0 )
	{
		// run normal sized ticks
		for( int tick = 0; tick < ticks_to_run; tick++ )
		{
			// tick entities
			for( std::map<unsigned long,Entity*>::iterator it = entities.begin(); it != entities.end(); it++ )
			{
				(*it).second->Tick( delta_tick );
				// reap dead
				if( (*it).second->dead )
				{
					// remove from alias map
					std::map<std::string,Entity*>::iterator alias_it = entities_by_alias.find( (*it).second->alias );
					if( alias_it != entities_by_alias.end() )
						entities_by_alias.erase( alias_it );

					// remove from layer
					RemoveEntityFromLayer( (*it).second );

					// delete
					delete (*it).second;

					// remove from main map
					std::map<unsigned long,Entity*>::iterator temp_it = it;
					it--;
					entities.erase( temp_it );
				}
			}

			// step physics
			cpSpaceStep( space, delta_tick_seconds );
		}
		// don't skip any time
		last_tick += ticks_to_run * delta_tick;
		return true;
	}
	return false;
}

unsigned long World::AddEntity( Entity* entity, int layer )
{
	// add to main map
	last_entity_id++;
	entities[last_entity_id] = entity;
	entity->SetID( last_entity_id );

	// add to alias map
	if( entity->alias.compare( "UNNAMED" ) != 0 )
	{
		// may want to warn if this is replacing an existing entity
		entities_by_alias[entity->alias] = entity;
	}

	// add to layer
	if( layer < 0 || layer >= SPIN_ENTITY_LAYERS )
	{
		fprintf( stderr, "World::AddEntity() -> invalid specified layer: %d, using max layer: %d instead...\n", layer, SPIN_ENTITY_LAYERS-1 );
		layer = SPIN_ENTITY_LAYERS-1;
	}
	layers[layer].push_back( entity );

	return last_entity_id;
}

Entity* World::GetEntity( unsigned long entity_id )
{
	std::map<unsigned long,Entity*>::iterator it = entities.find( entity_id );
	if( it == entities.end() )
		return 0;
	else
		return (*it).second;
}
Entity* World::GetEntityByAlias( const char* alias )
{
	std::string alias_string( alias );
	return GetEntityByAlias( alias_string );
}

Entity* World::GetEntityByAlias( std::string& alias )
{
	std::map<std::string,Entity*>::iterator it = entities_by_alias.find( alias );
	if( it == entities_by_alias.end() )
		return 0;
	else
		return (*it).second;
}

void World::RemoveEntityFromLayer( Entity* entity )
{
	// right now just search through each layer for the entity
	bool found_entity = false;
	for( int i = 0; i < SPIN_ENTITY_LAYERS && !found_entity; i++ )
	{
		for( std::vector<Entity*>::iterator it = layers[i].begin(); it != layers[i].end(); it++ )
		{
			if( (*it) == entity )
			{
				layers[i].erase( it );
				found_entity = true;
				break;
			}
		}
	}
}

bool World::TryLoadElement( TiXmlElement* element, bool& error )
{
	error = false;

	// check for NULL
	if( element == 0 )
	{
		fprintf( stderr, "World::TryLoadElement -> element was NULL!\n" );
		error = true;
		return false;
	}

	// entity
	if( strcmp( "entity", element->Value() ) == 0 )
	{
		if( !LoadEntity( element ) )
		{
			fprintf( stderr, "World::TryLoadElement -> unable to load entity element!\n" );
			error = true;
			return false;
		}
	}
	// unsupported
	else
		return false;
	
	return true;
}

int World::LoadEntity( const char* xml_path )
{
	// load document
	TiXmlDocument doc( xml_path );
	if( !doc.LoadFile() )
	{
		fprintf( stderr, "World::LoadEntity -> unable to load entity xml: %s\n", xml_path );
		return 0;
	}

	// find root
	TiXmlElement* root = doc.FirstChildElement();
	if( !root )
	{
		fprintf( stderr, "World::LoadEntity -> unable to find root tag in xml: %s\n", xml_path );
		return 0;
	}

	return LoadEntity( root );
}

int World::LoadEntity( TiXmlElement* element )
{
	// get entity type
	const char* type = element->Attribute( "type" );
	if( type == 0 )
	{
		fprintf( stderr, "World::LoadEntity -> no type attribute!\n" );
		return 0;
	}

	Entity* new_entity;

	bool entity_created = true;

	// QuadEntity
	if( strcmp( type, "quad" ) == 0 )
		new_entity = new QuadEntity();
	// TextEntity
	else if( strcmp( type, "text" ) == 0 )
		new_entity = new TextEntity();
	// BodyEntity
	else if( strcmp( type, "body" ) == 0 )
		new_entity = new BodyEntity();
	// static_body
	else if( strcmp( type, "static_body" ) == 0 )
	{
		new_entity = GetStaticBody();
		entity_created = false;
	}
	// SnapConstraint
	else if( strcmp( type, "snap_constraint" ) == 0 )
		new_entity = new SnapConstraint();
	else if( strcmp( type, "lua_entity" ) == 0 )
		new_entity = new LuaEntity();
	else
	{
		fprintf( stderr, "World::LoadEntity -> unsupported entity type: '%s!'\n", type );
		return 0;
	}

	if( new_entity->LoadElements( element ) )
	{
		// static body is already in the entity list and doesn't need to be added again
		if( entity_created )
			AddEntity( new_entity, new_entity->layer );
	}
	else
	{
		if( entity_created )
			delete new_entity;
		fprintf( stderr, "Entity::LoadElements() failed!\n" );
		return 0;
	}

	new_entity->GetID();
}
