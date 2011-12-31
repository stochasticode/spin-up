#include <World.h>
#include <SpinGame.h>
#include <Entity.h>
#include <ConstraintEntity.h>
#include <SpinXML.h>
#include <chipmunk.h>
#include <tinyxml.h>
#include <cstdio>
#include <sstream>

using namespace spin;

World::World(): space( cpSpaceNew() ), static_body( 0 ), delta_tick( 1000 / 80 ), delta_tick_seconds( delta_tick / 1000.0 )
{
	// set up space
	cpSpaceSetGravity( space, cpv( 0, -90 ) ); 
	cpSpaceSetIterations( space, 26 );
	cpSpaceSetDamping( space, 0.9 );

	cpSpaceAddCollisionHandler( space, World::COL_TYPE_GRAPPLE, World::COL_TYPE_SURFACE, 0, 0, GrappleGun::PostSolveGrapple, 0, 0);
	cpSpaceAddCollisionHandler( space, World::COL_TYPE_GRAPPLE, World::COL_TYPE_PROP, 0, 0, GrappleGun::PostSolveGrapple, 0, 0);
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
	entity->id = last_entity_id;

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

Entity* World::GetEntityByAlias( std::string& alias )
{
	std::map<std::string,Entity*>::iterator it = entities_by_alias.find( alias );
	if( it == entities_by_alias.end() )
		return 0;
	else
		return (*it).second;
}

bool World::LoadLevel( const char* xml_path )
{
	// load document
	TiXmlDocument doc( xml_path );
	if( !doc.LoadFile() )
	{
		fprintf( stdout, "Unable to load level xml: %s\n", xml_path );
		return false;
	}

	// find root
	TiXmlElement* root = doc.FirstChildElement( "spin_level" );
	if( !root )
	{
		fprintf( stdout, "Unable to find tag 'spin_levl' in  level xml: %s\n", xml_path );
		return false;
	}

	// iterate through children
	TiXmlElement* child = root->FirstChildElement();
	while( child != 0 )
	{
		// kevin 
		if( strcmp( "kevin", child->Value() ) == 0 )
		{
			// right now kevin is just like a vec2d, but could change in the future
			Vector kevin_position;
			std::string name;
			if( SpinXML::ReadVec2D( child, name, kevin_position ) )
				SPIN.kevin->SetPosition( kevin_position );
		}
		/*
		// surface
		else if( strcmp( "surface", child->Value() ) == 0 )
		{
			if( !AddSurfaceElement( child, Vector( 0.0, 0.0 ), 1.0 ) )
				return false;
		}
		*/
		// entity
		else if( strcmp( "entity", child->Value() ) == 0 )
		{
			Entity* new_entity;
			if( SpinXML::ReadEntity( child, &new_entity ) )
			{
				// static body doesn't need to be added to the entity list
				if( !dynamic_cast<StaticBody*>( new_entity ) )
					AddEntity( new_entity, 4 );
			}
		}

		child = child->NextSiblingElement();
	}
	return true;
}

/*
bool World::AddSurfaceElement( TiXmlElement* element, Vector position, float scale )
{
	// stringstream used to convert char* from XML to other types
	std::stringstream convert_stream;

	const char* x1_str = element->Attribute( "x1" );
	const char* y1_str = element->Attribute( "y1" );
	const char* x2_str = element->Attribute( "x2" );
	const char* y2_str = element->Attribute( "y2" );
			
	if( !x1_str || !y1_str || !x2_str || !y2_str )
	{
		fprintf( stdout, "Surface tag missing one of the attributes: x1, y1, x2, y2\n" );
		return false;
	}

	float x1 = 0;
	float y1 = 0;
	float x2 = 0;
	float y2 = 0;
	float radius = 1;
	float friction = 1;

	// convert coordinates
	convert_stream << x1_str << std::endl << y1_str << std::endl << x2_str << std::endl << y2_str << std::endl;
	convert_stream >> x1 >> y1 >> x2 >> y2;

	// get surface parameters
	TiXmlElement* surface_element = element->FirstChildElement( "surface_param" );
	while( surface_element != 0 )
	{
		const char* param_name = surface_element->Attribute( "name" );
		const char* param_value = surface_element->Attribute( "value" );
				
		if( !param_name || !param_value )
		{
			fprintf( stdout, "surface_param missing either name or value attribute\n" );
			return false;
		}

		// radius
		if( strcmp( param_name, "radius" ) == 0 )
		{
			convert_stream << param_value << std::endl;
			convert_stream >> radius;
		}
		// friction
		else if( strcmp( param_name, "friction" ) == 0 )
		{
			convert_stream << param_value << std::endl;
			convert_stream >> friction;
		}
		surface_element = surface_element->NextSiblingElement( "surface_param" );
	}

	AddEntity( new SurfaceEntity( position.x+scale*x1, position.y+scale*y1, position.x+scale*x2, position.y+scale*y2, scale*radius, friction ), 4 );
	return true;
}
*/

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
