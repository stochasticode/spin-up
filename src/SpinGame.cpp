#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <SpinGame.h>
#include <Kevin.h>
#include <Resources.h>
#include <Entity.h>
#include <SnapConstraint.h>
#include <Prop.h>
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
	kevin->SetPosition( Vector( -60, -68 ) );

	SnapConstraint* cord = new SnapConstraint( kevin, Vector( -60, -70 ) );
	world.AddEntity( cord );

	if( !world.LoadLevel( "assets/levels/test.xml" ) )
		return false;

	srand( time( 0 ) );
	for( int i = 0; i < 10; i++ )
	{
		BodyEntity* new_entity = BodyEntity::LoadEntity( "assets/entities/rock1.xml" );
		float scale_rand = (float)rand() / (float)RAND_MAX;
		new_entity->Scale( 0.05 + scale_rand * 0.35 );
		world.AddEntity( new_entity );
	}


	return true;
}

bool SpinGame::InitGraphics()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_TEXTURE_2D );
	glEnableClientState( GL_VERTEX_ARRAY );
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
	resources.LoadPNG( "assets/textures/rack.png", "rack" );
	resources.LoadPNG( "assets/textures/rock1.png", "rock1" );
	return true;
}

void SpinGame::Render()
{
	//glClear( GL_COLOR_BUFFER_BIT );
	camera.position_x = kevin->position.x;
	camera.position_y = kevin->position.y;
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
	if( key == 's' )
		kevin->grapple_gun.SwitchHook( 0 );
	if( key == 'd' )
		kevin->grapple_gun.SwitchHook( 1 );
	if( key == 'f' )
		kevin->grapple_gun.SwitchHook( 2 );
	if( key == 'a' )
		kevin->grapple_gun.DeactivateAllHooks();

	if( key == 'r' )
		BodyEntity::render_shapes = !BodyEntity::render_shapes;
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
