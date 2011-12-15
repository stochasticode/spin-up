#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <SpinGame.h>
#include <Kevin.h>
#include <Resources.h>
#include <Entity.h>
#include <cstdio>
#include <cstring>
#include <math.h>

using namespace spin;

SpinGame& SpinGame::Instance()
{
	static SpinGame instance;
	return instance;
}

bool SpinGame::Init( int argc, char** argv )
{
	// initialize graphics
	InitGraphics();

	// load textures, etc
	LoadResources();

	// set up camera
	camera.zoom = 3.0;

	kevin = new Kevin();
	world.AddEntity( kevin );

	if( !world.LoadLevel( "assets/levels/test.xml" ) )
		return false;


	return true;
}

bool SpinGame::InitGraphics()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_TEXTURE_2D );
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	return true;
}

bool SpinGame::LoadResources()
{
	resources.LoadPNG( "assets/textures/space.png", "space" );
	resources.LoadPNG( "assets/textures/kevin.png", "kevin" );
	resources.LoadPNG( "assets/textures/creature.png", "creature" );
	resources.LoadPNG( "assets/textures/burst.png", "burst" );
	resources.LoadPNG( "assets/textures/beagle.png", "beagle" );
	return true;
}

void SpinGame::Render()
{
	glClear( GL_COLOR_BUFFER_BIT );
	world.Render();
	glutSwapBuffers();
}

void SpinGame::Reshape( int width, int height )
{
	camera.Reshape( width, height );
	glutPostRedisplay();
}

void SpinGame::Idle()
{
	int milliseconds = glutGet( GLUT_ELAPSED_TIME );
	if( world.Tick( milliseconds ) )
		glutPostRedisplay();
}

void SpinGame::Keyboard( unsigned char key, int x, int y )
{
	if( key == 'w' )
		kevin->grapple_gun.SwitchHook( 0 );
	if( key == 'e' )
		kevin->grapple_gun.SwitchHook( 1 );
	if( key == 'r' )
		kevin->grapple_gun.SwitchHook( 2 );
}

void SpinGame::KeyboardUp( unsigned char key, int x, int y )
{
}

void SpinGame::Motion( int x, int y )
{
	mouse_x = x;
	// y is reversed from glut window coords
	mouse_y = camera.w_height-y;
	glutPostRedisplay();
}

void SpinGame::Mouse( int button, int state, int x, int y )
{
	int milliseconds = glutGet( GLUT_ELAPSED_TIME );

	if( state == GLUT_DOWN )
	{
		if( kevin != 0 )
		{
			float kevin_x = kevin->position.x;
			float kevin_y = kevin->position.y;

			camera.Transform( kevin_x, kevin_y );

			Vector direction;
			direction.x = ( mouse_x - camera.w_half_width ) - kevin_x;
			direction.y = ( mouse_y - camera.w_half_height ) - kevin_y;

			float direction_mag = sqrt( direction.x*direction.x + direction.y*direction.y );

			direction.x /= direction_mag;
			direction.y /= direction_mag;

			if( button == GLUT_LEFT_BUTTON )
				kevin->FirePrimary( direction  );
			else if( button == GLUT_RIGHT_BUTTON )
				kevin->FireSecondary( direction  );
		}
	}
}
