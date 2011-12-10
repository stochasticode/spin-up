#include <World.h>
#include <SpinGame.h>
#include <Surface.h>
#include <Entity.h>
#include <Tile.h>
#include <chipmunk.h>
#include <cstdio>

using namespace spin;

World::World(): space( cpSpaceNew() ), delta_tick( 1000 / 80 ), delta_tick_seconds( delta_tick / 1000.0 )
{
	// set up space
	cpSpaceSetGravity( space, cpv( 0, -70 ) ); 
	cpSpaceSetIterations( space, 8 );
}

World::~World()
{
	// delete space
	if( space != 0 )
		cpSpaceFree( space );

	// delete surfaces
	for( int i = 0; i < surfaces.size(); i++ )
	{
		if( surfaces[i] != 0 )
		{
			delete surfaces[i];
			surfaces[i] = 0;
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
}

void World::Render()
{
	// render tiles
	for( int i = 0; i < tiles.size(); i++ )
		tiles[i]->Render();

	// render surfaces
	for( int i = 0; i < surfaces.size(); i++ )
		surfaces[i]->Render();

	// render entities
	for( int i = 0; i < entities.size(); i++ )
		entities[i]->Render();


}

bool World::Tick( int milliseconds )
{
	int new_delta_tick = milliseconds - last_tick;
	int ticks_to_run = new_delta_tick / delta_tick;
	if( ticks_to_run > 0 )
	{
		// reap any dead entities 
		ReapEntities();

		// run normal sized ticks
		for( int tick = 0; tick < ticks_to_run; tick++ )
		{
			// tick entities
			for( int i = 0; i < entities.size(); i++ )
				entities[i]->Tick( delta_tick );
			// step physics
			cpSpaceStep( space, delta_tick_seconds );
		}
		// don't skip any time
		last_tick += ticks_to_run * delta_tick;
		return true;
	}
	return false;
}

Surface* World::AddSurface( Surface* surface )
{
	cpShape* shape = (cpShape*)surface->shape;
	shape->body = space->staticBody;
	cpSpaceAddShape( space, shape );
	surfaces.push_back( surface );
	return surface;
}

Entity* World::AddEntity( Entity* entity )
{
	cpSpaceAddBody( space, entity->body );
	cpSpaceAddShape( space, entity->shape );
	entities.push_back( entity );
	return entity;
}

Tile* World::AddTile( Tile* tile )
{
	tiles.push_back( tile );
	return tile;
}

void World::KillEntity( Entity* entity )
{
	dead_entities.push_back( entity );
}

void World::ReapEntities()
{
	for( std::vector<Entity*>::iterator it = dead_entities.begin(); it != dead_entities.end(); it++ )
	{
		for( std::vector<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++ )
		{
			if( *it == *it2 )
			{
				delete *it;
				entities.erase( it2 );
				break;
			}
		}
		dead_entities.erase( it );
		it--;
	}
}
