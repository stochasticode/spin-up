#include <GL/glew.h>
#include <GL/glut.h>
#include <SpinGame.h>
#include <Kevin.h>
#include <stdlib.h>
#include <cstdio>
#include <math.h>
#include <iostream>

using namespace spin;
using namespace std;

Kevin::Kevin(): foot_shape( 0 ), run_velocity( 100 ),  BodyEntity()
{
	if( !LoadXML( "assets/entities/kevin.xml" ) )
		fprintf( stderr, "Kevin::Kevin -> failed to load 'assets/entities/kevin.xml!\n" );

	if( shapes.size() != 2 )
		throw "Kevin::Kevin -> expected two shapes!";

	// set up control scheme
	SetScheme( "player" );

	foot_shape = shapes[1];
	foot_shape->e = 0;
}

Kevin::~Kevin()
{
}	

void Kevin::ProcessControlEvent( ControlEvent event )
{
	// move right
	if( event.function == CTRL_RIGHT )
	{
		running_right = event.is_down;

		if( running_right )
			foot_shape->surface_v = cpv( run_velocity, 0 );
		else if( running_left )
			foot_shape->surface_v = cpv( -run_velocity, 0 );
		else
			foot_shape->surface_v = cpv( 0, 0 );
	}

	else if( event.function == CTRL_LEFT )
	{
		running_left = event.is_down;

		if( running_left )
			foot_shape->surface_v = cpv( -run_velocity, 0 );
		else if( running_right )
			foot_shape->surface_v = cpv( run_velocity, 0 );
		else
			foot_shape->surface_v = cpv( 0, 0 );
	}

	else if( grounded && event.function == CTRL_UP && event.is_down )
	{
		ApplyImpulse( Vector( 0, 500 ) );
		cout << "body: " << position.x << ", " << position.y << endl;
	}
}

void Kevin::ProcessRawKeyEvent( RawKeyEvent event )
{
}

void Kevin::Tick( int milliseconds )
{
	// Grab the grounding normal from last frame
	cpVect ground_normal = cpvzero;
	cpBodyEachArbiter( body, (cpBodyArbiterIteratorFunc)SelectPlayerGroundNormal, &ground_normal );
	grounded = (ground_normal.y > 0.0);

	BodyEntity::Tick( milliseconds );
}

// stolen from Chipmunk Physics demo: Player.c
void Kevin::SelectPlayerGroundNormal( cpBody *body, cpArbiter *arb, cpVect *ground_normal )
{
	cpVect n = cpvneg( cpArbiterGetNormal( arb, 0 ) );
	
	if( n.y > ground_normal->y )
		(*ground_normal) = n;
}
