#ifndef SPIN_GRAPPLE_CONSTRAINT
#define SPIN_GRAPPLE_CONSTRAINT

#include <Entity.h>
#include <Vector.h>

namespace spin
{
	class GrappleConstraint: public ConstraintEntity
	{
		public:
		enum Type { GRAPPLE_WINCH, GRAPPLE_SPRINGY };

		protected:
		GrappleConstraint() {}
	};

	class GrappleConstraintWinch: public GrappleConstraint
	{
		public:
		GrappleConstraintWinch( BodyEntity* new_body_a, Vector static_anchor );
	};

	class GrappleConstraintSpringy: public GrappleConstraint
	{
		public:
		GrappleConstraintSpringy( BodyEntity* new_body_a, Vector static_anchor );
	};
}
#endif
