#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <SpinGame.h>
#include <Surface.h>
#include <Resources.h>
#include <Entity.h>
#include <cstdio>
#include <cstring>

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

	// set up background
	background.texture_key = "space";
	background.size.x = 1300;
	background.size.y = 1024;

	// set up camera
	camera.zoom = 3.0;


	// test stuff
	world.AddSurface( new Surface( 0, 0, 100, -100, 3, 1 ) );
	world.AddSurface( new Surface( -100, -100, 100, -100, 3, 1 ) );
	world.AddSurface( new Surface( -100, 100, 100, 100, 3, 1 ) );
	world.AddSurface( new Surface( -100, -100, -100, 100, 3, 1 ) );
	world.AddSurface( new Surface( 100, -100, 100, 100, 3, 1 ) );

	for( int i = 0; i < 100; i++ )
	{
		Entity* entity = world.AddEntity( new CircleEntity( 1, 5, 0.5 ) );
		entity->texture_key = "creature";
		entity->size.x = 10;
		entity->size.y = 10;
	}


	return true;
}

bool SpinGame::InitGraphics()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_TEXTURE_2D );
	return true;
}

bool SpinGame::LoadResources()
{
	resources.LoadPNG( "assets/space.png", "space" );
	resources.LoadPNG( "assets/creature.png", "creature" );
	return true;
}

void SpinGame::Render()
{
	// render background
	background.Render();

	// render world
	glPushMatrix();
	camera.Apply();
	world.Render();
	glPopMatrix();

	glutSwapBuffers();
}

void SpinGame::Reshape( int width, int height )
{
	camera.Reshape( width, height );
	glutPostRedisplay();
}

void SpinGame::Idle()
{
	// tick the world
	int milliseconds = glutGet( GLUT_ELAPSED_TIME );
	if( world.Tick( milliseconds ) )
		glutPostRedisplay();
}

void SpinGame::Keyboard( unsigned char key, int x, int y )
{
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
}
