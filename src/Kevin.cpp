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
}	

void Kevin::Render()
{
	BodyEntity::Render();
}

void Kevin::Tick( int milliseconds )
{
	BodyEntity::Tick( milliseconds );
}

void Kevin::FirePrimary( Vector direction )
{
	grapple_gun.ActivateHook( position, direction );
}

void Kevin::FireSecondary( Vector direction )
{
	grapple_gun.DeactivateHook();
}
