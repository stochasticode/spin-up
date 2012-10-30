#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <SpinGame.h>
#include <Resources.h>
#include <Entity.h>
#include <TextEditor.h>
#include <cstdio>
#include <cstring>
#include <math.h>
#include <iostream>

using namespace spin;
using namespace std;

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
	camera.zoom = 2.0;
	camera_mode = CAMERA_FOLLOWING;

	// create kevin object
	kevin = new Kevin();
	kevin->alias = "kevin";
	world.AddEntity( kevin, 6 );

	camera.position_x -= 30;
	camera.position_y -= 30;

	// load level
	if( !world.LoadXML( "assets/levels/test1.xml" ) )
	{
		fprintf( stderr, "Failed to load level!\n" );
		return false;
	}

	Entity* flask = world.GetEntityByAlias( "flask" );
	flask->LoadScript( "assets/scripts/flask.lua", true );

	// set up text_editor
	text_editor.SetText( flask->GetScriptText().c_str() );
	editing = false;

	// set up cursor body
	cursor_body = new BodyEntity();
	cursor_body->AddShapeCircle( 3, Vector( 0, 0 ), 0.0 );
	// it shouldn't collide with anything
	cursor_body->SetLayers(0);
	world.AddEntity( cursor_body, 6 );


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
		//camera.position_x = kevin->position.x;
		//camera.position_y = kevin->position.y;
	}

	world.Render();

	if( editing )
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
	{
    	cpVect point = MouseToSpace(mouse_x, mouse_y);
		cursor_body->SetPosition(Vector(point.x,point.y));
		glutPostRedisplay();
	}
}

void SpinGame::Keyboard( unsigned char key, int x, int y )
{
	if( editing )
		text_editor.KeyDown( key );
	else
	{
		if( key == 'r' )
			BodyEntity::render_shapes = !BodyEntity::render_shapes;
		else if( key == 'e' )
			editing = true;
		else if( kevin != 0 )
			kevin->KeyDown( key );
	}
}

void SpinGame::KeyboardUp( unsigned char key, int x, int y )
{
	if( editing )
		text_editor.KeyUp( key );
	else if( kevin != 0 )
		kevin->KeyUp( key );
}

void SpinGame::Motion( int x, int y )
{
	mouse_x = x;
	// y is reversed from glut window coords
	//mouse_y = camera.w_height-y;
	mouse_y = y;
	glutPostRedisplay();
}

void SpinGame::Mouse( int button, int state, int x, int y )
{
	cout << "(" << x << ", " << y << ")" << endl;
    cpVect point = MouseToSpace(x, y);
	cpShape *shape = cpSpacePointQueryFirst(world.GetCPSpace(), point, GRABABLE_MASK_BIT, CP_NO_GROUP);
	cout << "shape: " << shape << endl;
	/*
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
	*/
}

int SpinGame::GetDisplayWidth() { return display_width; }
int SpinGame::GetDisplayHeight() { return display_height; }

void SpinGame::TextEditorAction( EditorAction action )
{
	if( action == ED_WRITE )
	{
		cout << "WRITING..." << endl;

		Entity* flask = world.GetEntityByAlias( "flask" );
		flask->LoadScriptString( text_editor.GetText().c_str(), false );
	}
	else if( action == ED_QUIT )
	{
		cout << "QUITTING" << endl;
		editing = false;
	}
}


// taken from chipmuk physics ChipmunkDemo.c
cpVect SpinGame::MouseToSpace( int x, int y )
{
	// TODO: I'm sure there's a more efficient way to do this
	glPushMatrix();
	glLoadIdentity();
	camera.Apply();

	GLdouble model[16];
	glGetDoublev( GL_MODELVIEW_MATRIX, model );
	
	GLdouble proj[16];
	glGetDoublev( GL_PROJECTION_MATRIX, proj );
	
	GLint view[4];
	glGetIntegerv( GL_VIEWPORT, view );
	
	GLdouble mx, my, mz;
	gluUnProject( x, glutGet(GLUT_WINDOW_HEIGHT) - y, 0.0f, model, proj, view, &mx, &my, &mz );

	glPopMatrix();
	
	return cpv( mx, my );
}
