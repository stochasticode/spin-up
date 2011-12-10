#include <GL/glut.h>
#include <SpinGame.h>
#include <stdlib.h>
#include <cstdio>

using namespace spin;

void GLUT_Idle( void ) { SPIN.Idle(); }
void GLUT_Render( void ) { SPIN.Render(); }
void GLUT_Keyboard( unsigned char key, int x, int y ) { SPIN.Keyboard( key, x, y ); }
void GLUT_KeyboardUp( unsigned char key, int x, int y ) { SPIN.KeyboardUp( key, x, y ); }
void GLUT_Motion( int x,int y ) { SPIN.Motion( x, y ); }
void GLUT_Mouse( int button, int state, int x,int y ) { SPIN.Mouse( button, state, x, y ); }
void GLUT_Reshape( int width, int height ) { SPIN.Reshape( width, height ); }

int main( int argc, char** argv )
{
    glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
	glutInitWindowSize( 300, 300 );
	glutCreateWindow( "Spin" );
	glutDisplayFunc( &GLUT_Render );
	glutReshapeFunc( &GLUT_Reshape );
	glutKeyboardFunc( &GLUT_Keyboard );
	glutKeyboardUpFunc( &GLUT_KeyboardUp );
	glutMouseFunc( &GLUT_Mouse );
	glutMotionFunc( &GLUT_Motion );
	glutPassiveMotionFunc( &GLUT_Motion );
	glutIdleFunc( &GLUT_Idle );

	if( !SPIN.Init( argc, argv ) )
		return EXIT_FAILURE;
	else
	{
		glutMainLoop();
    	return EXIT_SUCCESS;
	}
}
