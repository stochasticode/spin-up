#include <Prop.h>
#include <World.h>
#include <chipmunk.h>

using namespace spin;

RectProp::RectProp( float mass, float width, float height, float friction ): Prop()
{
	InitBodyRect( mass, width, height, friction );
	shape->collision_type = World::COL_TYPE_PROP;
	size.x = width;
	size.y = height;
}

CircleProp::CircleProp( float mass, float radius, float friction ): Prop()
{
	InitBodyCircle( mass, radius, friction );
	shape->collision_type = World::COL_TYPE_PROP;
	size.x = 2*radius;
	size.y = 2*radius;
}
