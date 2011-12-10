#include <GL/glut.h>
#include <Quad.h>
#include <SpinGame.h>
#include <cstdio>

using namespace spin;

void Quad::Render()
{
	glPushMatrix();
	glTranslatef( position.x, position.y, 0 );
	glRotatef( rotation, 0, 0, 1 );
	glScalef( size.x*scale, size.y*scale, 1 );

	SPIN.resources.BindTexture( texture_key );

	glBegin(GL_TRIANGLES);
		glColor4f( color.r, color.g, color.b, color.a );
		glTexCoord2d( 0.0, 1.0 ); glVertex3f( -0.5, -0.5, 0.0 );
		glTexCoord2d( 1.0, 1.0 ); glVertex3f(  0.5, -0.5, 0.0 );
		glTexCoord2d( 0.0, 0.0 ); glVertex3f( -0.5,  0.5, 0.0 );

		glTexCoord2d( 1.0, 1.0 ); glVertex3f(  0.5, -0.5, 0 );
		glTexCoord2d( 0.0, 0.0 ); glVertex3f( -0.5,  0.5, 0 );
		glTexCoord2d( 1.0, 0.0 ); glVertex3f(  0.5,  0.5, 0 );
	glEnd();

	glPopMatrix();
}
