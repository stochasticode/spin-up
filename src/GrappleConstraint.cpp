#include <GrappleConstraint.h>
#include <math.h>

using namespace spin;

GrappleConstraintWinch::GrappleConstraintWinch( BodyEntity* new_body_a, Vector static_anchor )
{
	float dist_x = new_body_a->position.x-static_anchor.x;
	float dist_y = new_body_a->position.y-static_anchor.y;
	float dist = sqrt( dist_x*dist_x + dist_y*dist_y );
	InitConstraintSlideJoint( new_body_a, static_anchor, 0, dist );
}

GrappleConstraintSpringy::GrappleConstraintSpringy( BodyEntity* new_body_a, Vector static_anchor )
{
	float dist_x = new_body_a->position.x-static_anchor.x;
	float dist_y = new_body_a->position.y-static_anchor.y;
	float dist = sqrt( dist_x*dist_x + dist_y*dist_y );
	InitConstraintSpring( new_body_a, static_anchor, 0, 50, 1.0 );
}
