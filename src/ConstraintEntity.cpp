#include <ConstraintEntity.h>
#include <SpinGame.h>
#include <chipmunk.h>
#include <cstdio>

using namespace spin;

ConstraintEntity::ConstraintEntity(): constraint( 0 )
{
}

ConstraintEntity::~ConstraintEntity()
{
	ChipmunkCleanup();
}

void ConstraintEntity::ChipmunkCleanup()
{
	if( constraint != 0 )
	{
		cpSpaceRemoveConstraint( SPIN.world.GetCPSpace(), constraint );
		cpConstraintFree( constraint );
		constraint = 0;
	}
}

void ConstraintSlideJoint::Attach( BodyEntity* new_body_a, BodyEntity* new_body_b, Vector offset_a, Vector offset_b )
{
	ChipmunkCleanup();
	if( new_body_a != 0 && new_body_a->GetBody() != 0 && new_body_b != 0 && new_body_b->GetBody() != 0 )
	{
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpSlideJointNew( new_body_a->GetBody(), new_body_b->GetBody(), cpv( offset_a.x, offset_a.y ), cpv( offset_b.x, offset_b.y ), min_length, max_length ) );
		new_body_a->constraint_ids.push_back( id );
		new_body_b->constraint_ids.push_back( id );
	}
	else
		fprintf( stderr, "ConstraintSlideJoint::Attach -> failed due to null pointer!\n" );
}

// spring between two bodies
void ConstraintSpring::Attach( BodyEntity* new_body_a, BodyEntity* new_body_b, Vector offset_a, Vector offset_b )
{
	ChipmunkCleanup();
	if( new_body_a != 0 && new_body_a->GetBody()!= 0 && new_body_b != 0 && new_body_b->GetBody()!= 0 )
	{
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpDampedSpringNew( new_body_a->GetBody(), new_body_b->GetBody(), cpv( offset_a.x, offset_a.y ), cpv( offset_b.x, offset_b.y ), length, strength, damping ) );
		new_body_a->constraint_ids.push_back( id );
		new_body_b->constraint_ids.push_back( id );
	}
	else
		fprintf( stderr, "ConstraintSpring::Attach -> failed due to null pointer!\n" );
}
