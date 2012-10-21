#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <SpinGame.h>
#include <Kevin.h>
#include <Resources.h>
#include <Entity.h>
#include <TextEntity.h>
#include <SnapConstraint.h>
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
	// init world
	world.Init();

	// initialize graphics
	InitGraphics();

	// load textures, etc
	LoadResources();

	// set up camera
	camera.zoom = 4.0;
	camera_mode = CAMERA_FOLLOWING;

	// create kevin object
	kevin = new Kevin();
	kevin->alias = "kevin";
	world.AddEntity( kevin, 6 );

	// load level
	if( !world.LoadXML( "assets/levels/title.xml" ) )
	{
		fprintf( stderr, "Failed to load level!\n" );
		return false;
	}

	text_editor.SetText( "text in here\nand in here 2\n{something::->!}" );

	return true;
}

bool SpinGame::InitGraphics()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_ALPHA_TEST );
	glEnableClientState( GL_VERTEX_ARRAY );
	glClearColor( 0.1, 0.1, 0.1, 1.0 );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	return true;
}

bool SpinGame::LoadResources()
{
	resources.LoadPNG( "assets/textures/letter_s.png", "letter_s" );
	resources.LoadPNG( "assets/textures/letter_p.png", "letter_p" );
	resources.LoadPNG( "assets/textures/letter_i.png", "letter_i" );
	resources.LoadPNG( "assets/textures/letter_n.png", "letter_n" );


	resources.LoadPNG( "assets/textures/alphabet_ascii.png", "alphabet" );
	resources.LoadPNG( "assets/textures/space.png", "space" );
	resources.LoadPNG( "assets/textures/kevin.png", "kevin" );
	resources.LoadPNG( "assets/textures/creature.png", "creature" );
	resources.LoadPNG( "assets/textures/burst.png", "burst" );
	resources.LoadPNG( "assets/textures/beagle.png", "beagle" );
	resources.LoadPNG( "assets/textures/metal.png", "metal" );
	resources.LoadPNG( "assets/textures/stone.png", "stone" );
	resources.LoadPNG( "assets/textures/dirt.png", "dirt" );
	resources.LoadPNG( "assets/textures/crowbar.png", "crowbar" );
	resources.LoadPNG( "assets/textures/kevin.png", "kevin" );
	resources.LoadPNG( "assets/textures/flask.png", "flask" );
	resources.LoadPNG( "assets/textures/rack.png", "rack" );
	resources.LoadPNG( "assets/textures/rock1.png", "rock1" );
	resources.LoadPNG( "assets/textures/DEFAULT.png", "DEFAULT" );
	return true;
}

void SpinGame::Render()
{
	glClear( GL_COLOR_BUFFER_BIT );
	if( camera_mode == CAMERA_FOLLOWING )
	{
		camera.position_x = kevin->position.x;
		camera.position_y = kevin->position.y;
	}

	world.Render();
	text_editor.Render();

	glutSwapBuffers();
}

void SpinGame::Reshape( int width, int height )
{
	display_width = width;
	display_height = height;
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

int SpinGame::GetDisplayWidth() { return display_width; }
int SpinGame::GetDisplayHeight() { return display_height; }
