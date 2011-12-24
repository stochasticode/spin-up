#ifndef SPIN_SNAP_CONSTRAINT_H
#define SPIN_SNAP_CONSTRAINT_H

#include <Entity.h>
#include <Vector.h>

namespace spin
{
	class SnapConstraint: public ConstraintEntity
	{
		public:
		SnapConstraint( BodyEntity* new_body_a, Vector static_anchor );
		void Render();
		void Tick( int milliseconds );
	};
}


#endif
