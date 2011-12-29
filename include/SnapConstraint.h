#ifndef SPIN_SNAP_CONSTRAINT_H
#define SPIN_SNAP_CONSTRAINT_H

#include <Entity.h>
#include <Vector.h>

namespace spin
{
	class SnapConstraint: public ConstraintEntity
	{
		public:
		SnapConstraint();
		void Init( BodyEntity* new_body_a, Vector static_anchor, float strength, float new_max_length );

		void Render();
		void Tick( int milliseconds );

		//static bool ReadSnapConstraint( TiXmlElement* element, SnapConstraint** constraint_out );
		bool LoadXML( TiXmlElement* element );

		private:
		float max_length;
	};
}


#endif
