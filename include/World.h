#ifndef GAL_WORLD_H
#define GAL_WORLD_H

#include <Vector.h>
#include <vector>
#include <Entity.h>

struct cpSpace;
struct cpShape;
struct cpBody;
struct TiXmlElement;

namespace spin
{
	class Entity;

	class World
	{
		public:
		World();
		~World();

		enum CollisionType { COL_TYPE_OTHER, COL_TYPE_SURFACE, COL_TYPE_PROJECTILE, COL_TYPE_GRAPPLE };

		bool Tick( int milliseconds );
		void Render();

		cpSpace* GetCPSpace() { return space; }

		Entity* AddEntity( Entity* entity );

		bool LoadLevel( const char* xml_path );
		bool AddSurfaceElement( TiXmlElement* element, Vector position, float scale );

		private:
		cpSpace* space;
		QuadEntity background;
		int delta_tick;
		float delta_tick_seconds;
		int last_tick;

		std::vector<Entity*> entities;
	};
}
#endif
