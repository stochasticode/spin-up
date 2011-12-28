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
	if( !LoadXML( "assets/entities/kevin.xml" ) )
		fprintf( stderr, "Kevin::Kevin -> failed to load 'assets/entities/kevin.xml!\n" );

	grapple_gun.SetGrappleType( 0, GRAPPLE_WINCH );
}

Kevin::~Kevin()
{
}	

void Kevin::FirePrimary( Vector direction )
{
	grapple_gun.ActivateHook( position, direction );
}

void Kevin::FireSecondary( Vector direction )
{
	grapple_gun.DeactivateHook();
}
