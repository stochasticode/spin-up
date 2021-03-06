#include <GrappleGun.h>
#include <SpinGame.h>
#include <World.h>
#include <Entity.h>
#include <chipmunk.h>
#include <cstdio>

using namespace spin;

GrappleHook::GrappleHook( int new_life_left, GrappleGun* new_parent_gun, GrappleInfo new_info ): BodyEntity(), life_left( new_life_left ), info( new_info )
{
	if( !LoadXML( "assets/entities/grapple_hook.xml" ) )
		fprintf( stderr, "GrappleHook::GrappleHook -> failed to load 'assets/entities/grapple_hook.xml!\n" );

	// set collision type
	if( shapes.size() > 0 )
		shapes[0]->collision_type = World::COL_TYPE_GRAPPLE;
	else
		fprintf( stderr, "GrappleHook::GrappleHook -> couldn't set collision_type because there were no shapes!\n" );

	// set quad color
	if( quads.size() > 0 )
		quads[0].color = new_info.color;
	else
		fprintf( stderr, "GrappleHook::GrappleHook -> couldn't set color because there were no quads!\n" );
}

void GrappleHook::Render()
{
	Kevin* kevin = SPIN.kevin;
	
	glDisable( GL_TEXTURE_2D );
	if( info.parent_gun->GetCurrentHook() == info.hook_index )
		glLineWidth( 6.0 );
	glBegin( GL_LINES );
		glColor4f( info.color.r, info.color.g, info.color.b, 0.5 );
		glVertex3f( position.x, position.y, 0 );
		glVertex3f( kevin->position.x, kevin->position.y, 0 );
	glEnd();
	glLineWidth( 1.0 );
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

GrappleGun::GrappleGun(): current_hook( 0 )
{
	for( int i = 0; i < MAX_GRAPPLE_HOOKS; i++ )
	{
		grapple_info[i].hook_index = i;
		grapple_info[i].parent_gun = this;
		if( i == 0 )
			grapple_info[i].color = Color( 1.0, 0.0, 0.0, 1.0 );
		else if( i == 1 )
			grapple_info[i].color = Color( 0.0, 1.0, 0.0, 1.0 );
		else if( i == 2 )
			grapple_info[i].color = Color( 0.0, 0.0, 1.0, 1.0 );
	}
}

void GrappleGun::SetGrappleType( int hook_index, GrappleConstraintType type )
{
	if( hook_index < 0 || hook_index >= MAX_GRAPPLE_HOOKS )
		fprintf( stderr, "GrappleGun::SetGrappleType -> invalid hook_index: %d, max hooks: %d!\n", hook_index, MAX_GRAPPLE_HOOKS );
	else
		grapple_info[hook_index].type = type;
}

void GrappleGun::SwitchHook( int hook_index )
{
	if( hook_index < 0 || hook_index >= MAX_GRAPPLE_HOOKS )
		fprintf( stderr, "GrappleGun::SwitchHook -> invalid hook_index: %d, max hooks: %d!\n", hook_index, MAX_GRAPPLE_HOOKS );
	else
		current_hook = hook_index;
}

void GrappleGun::ActivateHook( Vector position, Vector direction )
{
	DeactivateHook();

	GrappleHook* hook = new GrappleHook( 800, this, grapple_info[current_hook] );
	grapple_info[current_hook].hook_id = SPIN.world.AddEntity( hook, 5 );
	hook->SetPosition( Vector( position.x, position.y ) );
	hook->SetVelocity( Vector( 300*direction.x, 300*direction.y ) );
}

void GrappleGun::DeactivateHook()
{
	DeactivateHook( current_hook );
}

void GrappleGun::DeactivateHook( int hook_index )
{
	GrappleInfo info = grapple_info[hook_index];

	// kill entity if it exists
	Entity* old_hook = SPIN.world.GetEntity( info.hook_id );
	if( old_hook != 0 )
		old_hook->dead = true;

	// kill constraint if it exists
	ConstraintEntity* old_constraint = (ConstraintEntity*)SPIN.world.GetEntity( info.constraint_id );
	if( old_constraint != 0 )
		old_constraint->dead = true;

	grapple_info[hook_index].hook_id = 0;
	grapple_info[hook_index].constraint_id = 0;
}

void GrappleGun::DeactivateAllHooks()
{
	for( int i = 0; i < MAX_GRAPPLE_HOOKS; i++ )
		DeactivateHook( i );
}

//cpSpaceAddCollisionHandler( space, World::COL_TYPE_GRAPPLE, World::COL_TYPE_SURFACE, 0, 0, GrappleGun::PostSolveGrapple, 0, 0);
void GrappleGun::PostSolveGrapple( cpArbiter *arb, cpSpace *space, void *unused )
{
	cpShape *a; // COL_TYPE_GRAPPLE || COL_TYPE_PROP
	cpShape *b; // COL_TYPE_SURFACE
	cpArbiterGetShapes( arb, &a, &b );

	if( a->data != 0 )
	{
		GrappleHook* grapple = (GrappleHook*)a->data;
		grapple->dead = true;

		// prop
		BodyEntity* prop= dynamic_cast<BodyEntity*>((Entity*)b->data);
		if( prop != 0 )
			cpSpaceAddPostStepCallback( space, (cpPostStepFunc)GrappleGun::PostStepGrapple, a, b );
	}
}

void GrappleGun::PostStepGrapple( cpSpace* space, cpShape* shape, cpShape* prop_shape )
{
	if( shape->data != 0 )
	{
		GrappleHook* grapple = (GrappleHook*)shape->data;
		GrappleGun* gun = grapple->info.parent_gun;
		gun->DeactivateHook( grapple->info.hook_index );
		ConstraintEntity* constraint = 0;

		// hit prop
		if( prop_shape != 0 )
		{
			BodyEntity* prop = (BodyEntity*)prop_shape->data;

			Vector offset;
			float cos_a = cos( -prop->GetRotation() );
			float sin_a = sin( -prop->GetRotation() );

			float offset_x = grapple->position.x - prop->position.x;
			float offset_y = grapple->position.y - prop->position.y;

			offset.x = offset_x * cos_a - offset_y * sin_a;
			offset.y = offset_x * sin_a + offset_y * cos_a;

			switch( grapple->info.type )
			{
				case GRAPPLE_WINCH:
				{
					GrappleConstraintWinch* winch = new GrappleConstraintWinch( grapple->info );
					float dist_x = SPIN.kevin->position.x-grapple->position.x;
					float dist_y = SPIN.kevin->position.y-grapple->position.y;
					winch->max_length = sqrt( dist_x*dist_x + dist_y*dist_y );
					constraint = winch;
					constraint->Attach( SPIN.kevin, prop, Vector( 0.0, 0.0 ), offset );
					break;
				}
				default:
					GrappleConstraintSpringy* springy = new GrappleConstraintSpringy( grapple->info );
					springy->strength= 60;
					springy->damping = 10.0;
					constraint = springy;
					constraint->Attach( SPIN.kevin, prop, Vector( 0.0, 0.0 ), offset );
			}
	
			unsigned long new_constraint_id = SPIN.world.AddEntity( constraint, 5 );
			gun->grapple_info[grapple->info.hook_index].constraint_id = new_constraint_id;
		}
	}
}

void GrappleConstraintSpringy::Render()
{
	GrappleConstraint::Render( constraint );
}

void GrappleConstraintWinch::Render()
{
	GrappleConstraint::Render( constraint );
}
	
void GrappleConstraint::Render( cpConstraint* own_constraint)
{
	// hmmm, this works for slide joints too but probably isn't the best way to get the two bodies...
	cpDampedSpring* spring = (cpDampedSpring*)own_constraint;
	cpBody* a = spring->constraint.a;
	cpBody* b = spring->constraint.b;

	float cos_a = cos( a->a );
	float sin_a = sin( a->a );
	float cos_b = cos( b->a );
	float sin_b = sin( b->a );
	float anchor_x = 0.0;
	float anchor_y = 0.0;

	// rope
	if( info.parent_gun->GetCurrentHook() == info.hook_index )
		glLineWidth( 6.0 );
	glDisable( GL_TEXTURE_2D );
	glBegin( GL_LINES );
		glColor4f( info.color.r, info.color.g, info.color.b, 0.5 );

		anchor_x = spring->anchr1.x * cos_a - spring->anchr1.y * sin_a;
		anchor_y = spring->anchr1.x * sin_a + spring->anchr1.y * cos_a;
		glVertex3f( cpBodyGetPos( a ).x + anchor_x, cpBodyGetPos( a ).y + anchor_y, 0 );

		anchor_x = spring->anchr2.x * cos_b - spring->anchr2.y * sin_b;
		anchor_y = spring->anchr2.x * sin_b + spring->anchr2.y * cos_b;
		glVertex3f( cpBodyGetPos( b ).x + anchor_x, cpBodyGetPos( b ).y + anchor_y, 0 );
	glEnd();
	glEnable( GL_TEXTURE_2D );
	glLineWidth( 1.0 );

	// hook
	glPushMatrix();
	glTranslatef( cpBodyGetPos( b ).x + anchor_x, cpBodyGetPos( b ).y + anchor_y, 0 );
	SPIN.resources.BindTexture( "burst" );
	glBegin(GL_TRIANGLES);
		glColor4f( info.color.r, info.color.g, info.color.b, 0.5 );
		glTexCoord2d( 0.0, 1.0 ); glVertex3f( -5.0, -5.0, 0.0 );
		glTexCoord2d( 1.0, 1.0 ); glVertex3f(  5.0, -5.0, 0.0 );
		glTexCoord2d( 0.0, 0.0 ); glVertex3f( -5.0,  5.0, 0.0 );

		glTexCoord2d( 1.0, 1.0 ); glVertex3f(  5.0, -5.0, 0 );
		glTexCoord2d( 0.0, 0.0 ); glVertex3f( -5.0,  5.0, 0 );
		glTexCoord2d( 1.0, 0.0 ); glVertex3f(  5.0,  5.0, 0 );
	glEnd();
	glPopMatrix();
}
