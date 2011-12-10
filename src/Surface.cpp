#include <GL/glut.h>
#include <Surface.h>
#include <World.h>
#include <chipmunk.h>

using namespace spin;

Surface::Surface( float x1, float y1, float x2, float y2, float new_radius, float friction ): radius( new_radius )
{
	shape = (cpSegmentShape*)cpSegmentShapeNew( 0, cpv( x1, y1 ), cpv( x2, y2 ), radius );
	shape->shape.collision_type = World::COL_TYPE_SURFACE;
	cpShapeSetFriction( (cpShape*)shape, friction );
}

void Surface::Render()
{
	glDisable( GL_TEXTURE_2D );
	glBegin(GL_LINES);
		glColor3f( 1, 1, 1 );
		glVertex3f( shape->a.x, shape->a.y, 0 );
		glVertex3f( shape->b.x, shape->b.y, 0 );
	glEnd();
	glEnable( GL_TEXTURE_2D );
}
