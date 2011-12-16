#ifndef SPIN_PROP_H
#define SPIN_PROP_H

#include <Entity.h>

namespace spin
{
	class Prop: public BodyEntity
	{
		protected:
		Prop(): BodyEntity() {}
	};

	class RectProp: public Prop
	{
		public:
		RectProp( float mass, float width, float height, float friction );
	};

	class CircleProp: public Prop
	{
		CircleProp( float mass, float radius, float friction );
	};

}

#endif
