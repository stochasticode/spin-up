#ifndef GAL_WORLD_H
#define GAL_WORLD_H

#include <Vector.h>
#include <Entity.h>
#include <SpinXML.h>
#include <vector>
#include <map>

struct cpSpace;
struct TiXmlElement;

#ifndef SPIN_ENTITY_LAYERS
	#define SPIN_ENTITY_LAYERS 8
#endif

namespace spin
{
	class Entity;
	class StaticBody;

	class World: public SpinXML
	{
		public:
		World();
		~World();

		void Init();

		enum CollisionType { COL_TYPE_OTHER, COL_TYPE_SURFACE, COL_TYPE_PROP, COL_TYPE_PROJECTILE, COL_TYPE_GRAPPLE };

		bool Tick( int milliseconds );
		void Render();

		cpSpace* GetCPSpace() { return space; }
		StaticBody* GetStaticBody() { return static_body; }

		unsigned long AddEntity( Entity* entity, int layer );
		Entity* GetEntity( unsigned long entity_id );
		Entity* GetEntityByAlias( std::string& alias );
		Entity* GetEntityByAlias( const char* alias );

		virtual bool TryLoadElement( TiXmlElement* element, bool& error );
		virtual const char* GetDescription() { return "World"; }

		int LoadEntity( const char* xml_path );
		int LoadEntity( TiXmlElement* element );

		private:
		cpSpace* space;
		StaticBody* static_body;
		int delta_tick;
		float delta_tick_seconds;
		int last_tick;

		std::map<unsigned long,Entity*> entities;
		std::map<std::string,Entity*> entities_by_alias;
		std::vector<Entity*> layers[SPIN_ENTITY_LAYERS];
		unsigned long last_entity_id;

		void RemoveEntityFromLayer( Entity* entity );
	};
}
#endif
