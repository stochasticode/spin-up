#ifndef SPIN_ENTITY_H
#define SPIN_ENTITY_H

#include <Quad.h>
#include <World.h>
#include <chipmunk.h>

struct cpBody;
struct cpShape;

namespace spin
{
	// should be abstract or we need to implement the constructor
	class Entity: public Quad
	{
		public:
		virtual ~Entity();
		virtual void Tick( int milliseconds );

		void GetVelocity( float& x, float& y );

		friend class World;

		protected:
		cpBody *body;
		cpShape *shape;
	};

	class CircleEntity: public Entity
	{
		public:
		CircleEntity( float mass, float radius, float friction );
		virtual ~CircleEntity() {}
	};
}

#endif
