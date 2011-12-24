#include <SnapConstraint.h>
#include <stdio.h>

using namespace spin;

SnapConstraint::SnapConstraint( BodyEntity* new_body_a, Vector static_anchor ): ConstraintEntity()
{

	InitConstraintSpring( new_body_a, static_anchor, 0, 300, 10 );
}

void SnapConstraint::Render()
{
	cpDampedSpring* spring = (cpDampedSpring*)constraint;
	cpBody* a = spring->constraint.a;
	cpBody* b = spring->constraint.b;

	float cos_a = cos( a->a );
	float sin_a = sin( a->a );
	float cos_b = cos( b->a );
	float sin_b = sin( b->a );
	float anchor_x = 0.0;
	float anchor_y = 0.0;

	// rope
	glLineWidth( 6.0 );
	glDisable( GL_TEXTURE_2D );
	glBegin( GL_LINES );
		glColor4f( 1.0, 1.0, 1.0, 0.5 );

		anchor_x = spring->anchr1.x * cos_a - spring->anchr1.y * sin_a;
		anchor_y = spring->anchr1.x * sin_a + spring->anchr1.y * cos_a;
		glVertex3f( cpBodyGetPos( a ).x + anchor_x, cpBodyGetPos( a ).y + anchor_y, 0 );

		anchor_x = spring->anchr2.x * cos_b - spring->anchr2.y * sin_b;
		anchor_y = spring->anchr2.x * sin_b + spring->anchr2.y * cos_b;
		glVertex3f( cpBodyGetPos( b ).x + anchor_x, cpBodyGetPos( b ).y + anchor_y, 0 );
	glEnd();
	glEnable( GL_TEXTURE_2D );
	glLineWidth( 1.0 );
}

void SnapConstraint::Tick( int milliseconds )
{
	cpDampedSpring* spring = (cpDampedSpring*)constraint;
	cpBody* a = spring->constraint.a;
	cpBody* b = spring->constraint.b;

	float cos_a = cos( a->a );
	float sin_a = sin( a->a );
	float cos_b = cos( b->a );
	float sin_b = sin( b->a );
	float anchor_x = 0.0;
	float anchor_y = 0.0;

	anchor_x = spring->anchr1.x * cos_a - spring->anchr1.y * sin_a;
	anchor_y = spring->anchr1.x * sin_a + spring->anchr1.y * cos_a;
	float body_a_x = cpBodyGetPos( a ).x + anchor_x;
	float body_a_y = cpBodyGetPos( a ).y + anchor_y;

	anchor_x = spring->anchr2.x * cos_b - spring->anchr2.y * sin_b;
	anchor_y = spring->anchr2.x * sin_b + spring->anchr2.y * cos_b;
	float body_b_x = cpBodyGetPos( b ).x + anchor_x;
	float body_b_y = cpBodyGetPos( b ).y + anchor_y;

	float length = sqrt( (body_a_x-body_b_x)*(body_a_x-body_b_x) + (body_a_y-body_b_y)*(body_a_y-body_b_y) );
	if( length > 45 )
		dead = true;
}
