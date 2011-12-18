#include <Prop.h>
#include <World.h>
#include <chipmunk.h>

using namespace spin;

CircleProp::CircleProp( float mass, float radius, float friction ): BodyEntity()
{
	InitBodyCircle( mass, radius, friction );
	shape->collision_type = World::COL_TYPE_PROP;
	size.x = 2*radius;
	size.y = 2*radius;
}

PolyProp::PolyProp( float mass, float width, float height, std::vector<Vector> points, float friction ): BodyEntity()
{
	InitBodyPoly( mass, points, friction );
	shape->collision_type = World::COL_TYPE_PROP;
	size.x = width;
	size.y = height;
}

PolyProp::PolyProp( float mass, float width, float height, float friction ): BodyEntity()
{
	InitBodyRect( mass, width, height, friction );
	shape->collision_type = World::COL_TYPE_PROP;
	size.x = width;
	size.y = height;
}
