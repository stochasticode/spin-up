#include <SpinGame.h>
#include <World.h>
#include <Entity.h>

using namespace spin;

Entity::~Entity()
{
	if( body != 0 )
	{
		cpSpaceRemoveBody( SPIN.world.GetSpace(), body );
		cpBodyFree( body );
		body = 0;
	}
	if( shape != 0 )
	{
		cpSpaceRemoveShape( SPIN.world.GetSpace(), shape );
		cpShapeFree( shape );
		shape = 0;
	}
}

void Entity::Tick( int milliseconds )
{
	if( body != 0 )
	{
		position.x = cpBodyGetPos( body ).x;
		position.y = cpBodyGetPos( body ).y;
		rotation = body->a * 180 / M_PI;
	}
}

void Entity::GetVelocity( float& x, float& y )
{
	if( body != 0 )
	{
		x = body->v.x;
		y = body->v.y;
	}
	else
	{
		x = 0.0;
		y = 0.0;
	}
}

CircleEntity::CircleEntity( float mass, float radius, float friction )
{
	// set up body
	body = cpBodyNew( mass, cpMomentForCircle( mass, 0, radius, cpvzero ) );
	cpBodySetPos( body, cpv( 0, 0 ) );

	// set up shape
	shape = cpCircleShapeNew( body, radius, cpvzero );
	cpShapeSetFriction( shape, friction );

	// link body to entity
	shape->data = this;
}
