#include <GL/glew.h>
#include <GL/glut.h>
#include <SpinGame.h>
#include <Kevin.h>
#include <stdlib.h>
#include <cstdio>
#include <math.h>

using namespace spin;

Kevin::Kevin(): BodyEntity()
{
	size.x = 12.0;
	size.y = 12.0; 

	InitBodyCircle( 10.0, 6.0, 5.0 );

	texture_key = "beagle";

	grapple_gun.SetGrappleType( 0, GRAPPLE_WINCH );
}

Kevin::~Kevin()
{
	/*
	// delete grapple constraint
	if( grapple_constraint != 0 )
	{
		cpConstraintFree( grapple_constraint );
		grapple_constraint = 0;
	}
	*/
}	

void Kevin::Render()
{
	/*
	// grapple
	if( grapple_constraint != 0 )
	{
		glDisable( GL_TEXTURE_2D );
		glLineWidth( 3 );
		glColor3f( 0.0, 1.0, 1.0 );
		glBegin(GL_LINES );
			glVertex3f( position.x, position.y, 0 );
			glVertex3f( grapple_x, grapple_y, 0 );
		glEnd();
		glLineWidth( 1 );
		glEnable( GL_TEXTURE_2D );
	}
	*/

	//grapple_gun.Render();
	BodyEntity::Render();
}

void Kevin::Tick( int milliseconds )
{
	BodyEntity::Tick( milliseconds );
	//grapple_gun.Tick( milliseconds );
}

void Kevin::FirePrimary( Vector direction )
{
	grapple_gun.ActivateHook( position, direction );
}

void Kevin::FireSecondary( Vector direction )
{
	grapple_gun.DeactivateHook();
}

/*
void Kevin::EnableGrapple( float x, float y )
{
	if( grapple_constraint != 0 )
		DisableGrapple();
	float body_x = cpBodyGetPos( body ).x;
	float body_y = cpBodyGetPos( body ).y;
	float grapple_length = sqrt((body_x-x)*(body_x-x) + (body_y-y)*(body_y-y)) ;
	grapple_constraint = cpSpaceAddConstraint( Game::Instance().world.space, cpSlideJointNew( Game::Instance().world.space->staticBody, body, cpv( x, y ), cpvzero, 0, grapple_length ));
	grapple_x = x;
	grapple_y = y;
}

void Kevin::DisableGrapple()
{
	if( grapple_constraint != 0 )
	{
		cpSpaceRemoveConstraint( Game::Instance().world.space, grapple_constraint );
		cpConstraintFree( grapple_constraint );
		grapple_constraint = 0;
	}
}
*/
