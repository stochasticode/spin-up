#ifndef SPIN_PROP_H
#define SPIN_PROP_H

#include <Entity.h>
#include <Vector.h>

namespace spin
{
	class CircleProp: public BodyEntity
	{
		public:
		CircleProp( float mass, float radius, float friction );
	};

	class PolyProp: public BodyEntity
	{
		public:
		PolyProp( float mass, float width, float height, std::vector<Vector> points, float friction );
		PolyProp( float mass, float width, float height, float friction );
	};
}

#endif
