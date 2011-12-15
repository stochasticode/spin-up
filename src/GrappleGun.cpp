#include <GrappleGun.h>
#include <SpinGame.h>
#include <World.h>
#include <Entity.h>
#include <chipmunk.h>
#include <cstdio>

using namespace spin;

GrappleHook::GrappleHook( int new_life_left, GrappleGun* new_parent_gun ): BodyEntity(), life_left( new_life_left ), parent_gun( new_parent_gun )
{
	InitBodyCircle( 1, 1, 0 );
	shape->collision_type = World::COL_TYPE_GRAPPLE;
	size.x = 5;
	size.y = 5;
	texture_key = "burst";
}

void GrappleHook::Render()
{
	Kevin* kevin = SPIN.kevin;
	
	glDisable( GL_TEXTURE_2D );
	glBegin( GL_LINES );
		glColor3f( 1.0, 1.0, 1.0 );
		glVertex3f( position.x, position.y, 0 );
		glVertex3f( kevin->position.x, kevin->position.y, 0 );
		//glVertex3f( 1000, 1000, 0 );
	glEnd();
	glEnable( GL_TEXTURE_2D );
	BodyEntity::Render();
}

void GrappleHook::Tick( int milliseconds )
{
	BodyEntity::Tick( milliseconds );
	life_left -= milliseconds;
	if( life_left < 0 )
		dead = true;
}

GrappleGun::GrappleGun( int new_max_hooks ): max_hooks( new_max_hooks ), current_hook( 0 )
{
	if( max_hooks < 0 )
		max_hooks = 0;

	for( int i = 0; i < max_hooks; i++ )
	{
		hooks.push_back( 0 );
		constraints.push_back( 0 );
	}
}

void GrappleGun::ActivateHook( Vector position, Vector direction )
{
	DeactivateHook();

	GrappleHook* hook = new GrappleHook( 800, this );
	unsigned long hook_id = SPIN.world.AddEntity( hook );
	hook->SetPosition( Vector( position.x, position.y ) );
	hook->SetVelocity( Vector( 300*direction.x, 300*direction.y ) );

	hooks[current_hook] = hook_id;
}

void GrappleGun::DeactivateHook()
{
	fflush( stdout );
	if( hooks[current_hook] != 0 )
	{
		// kill entity if it exists
		Entity* old_hook = SPIN.world.GetEntity( hooks[current_hook] );
		if( old_hook != 0 )
			old_hook->dead = true;
		// kill constraint if it exists
		ConstraintEntity* old_constraint = (ConstraintEntity*)SPIN.world.GetEntity( constraints[current_hook] );
		if( old_constraint != 0 )
			old_constraint->dead = true;
	}
}

void GrappleGun::Render()
{
}

//cpSpaceAddCollisionHandler( space, World::COL_TYPE_GRAPPLE, World::COL_TYPE_SURFACE, 0, 0, GrappleGun::PostSolveGrapple, 0, 0);
void GrappleGun::PostSolveGrapple( cpArbiter *arb, cpSpace *space, void *unused )
{
	printf( "GRAPPLE\n" );
	fflush( stdout );

	cpShape *a, *b;
	cpArbiterGetShapes(arb, &a, &b);

	if( a->data != 0 )
	{
		GrappleHook* grapple = (GrappleHook*)a->data;
		grapple->dead = true;
		cpSpaceAddPostStepCallback( space, (cpPostStepFunc)GrappleGun::PostStepGrapple, a, 0 );
	}
}

void GrappleGun::PostStepGrapple( cpSpace* space, cpShape* shape, void* unused )
{
	if( shape->data != 0 )
	{

		GrappleHook* grapple = (GrappleHook*)shape->data;
		GrappleGun* gun = grapple->parent_gun;

		ConstraintEntity* old_constraint = (ConstraintEntity*)SPIN.world.GetEntity( gun->constraints[gun->current_hook] );
		if( old_constraint != 0 )
			old_constraint->dead = true;

		ConstraintEntity* constraint = new ConstraintEntity();
		unsigned long new_constraint_id = SPIN.world.AddEntity( constraint );
		constraint->InitConstraintSlideJoint( SPIN.kevin, Vector( grapple->position.x, grapple->position.y ), 0, 100 );
		gun->constraints[gun->current_hook] = new_constraint_id;
	}
}
