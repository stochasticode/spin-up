//#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <Camera.h>
#include <math.h>
#include <cstdio>

using namespace spin;

void Camera::Apply()
{
	glScalef( zoom, zoom, 1 );
	glTranslatef( -position_x, -position_y, 0 );
}

void Camera::Reshape( int width, int height )
{
	glViewport( 0, 0, width, height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	w_width = width;
	w_height = height;
	w_half_width = width / 2;
	w_half_height = height / 2;

	gluOrtho2D(-w_half_width, w_half_width, -w_half_height, w_half_height );
	glMatrixMode(GL_MODELVIEW);
}

void Camera::Transform( float& x, float& y )
{
	x -= position_x;
	y -= position_y;
	if( fabs( zoom ) > 1e-20 )
	{
		x *= zoom;
		y *= zoom;
	}
}
