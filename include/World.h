#ifndef GAL_WORLD_H
#define GAL_WORLD_H

//#include <Entity.h>
//#include <Surface.h>
#include <vector>
//#include <Tile.h>

struct cpSpace;
struct cpShape;
struct cpBody;

namespace spin
{
	class Entity;
	class Surface;
	class Tile;

	class World
	{
		public:
		World();
		~World();

		enum CollisionType { COL_TYPE_OTHER, COL_TYPE_SURFACE };

		void Render();
		bool Tick( int milliseconds );
		Surface* AddSurface( Surface* surface );
		Entity* AddEntity( Entity* entity );
		Tile* AddTile( Tile* entity );
		void KillEntity( Entity* entity );

		cpSpace* GetSpace() { return space; }

		private:
		cpSpace* space;
		int delta_tick;
		float delta_tick_seconds;
		int last_tick;

		std::vector<Surface*> surfaces;
		std::vector<Entity*> entities;
		std::vector<Entity*> dead_entities;
		std::vector<Tile*> tiles;

		void ReapEntities();
	};
}
#endif
