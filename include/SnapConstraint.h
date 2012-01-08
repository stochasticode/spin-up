#ifndef SPIN_SNAP_CONSTRAINT_H
#define SPIN_SNAP_CONSTRAINT_H

#include <ConstraintEntity.h>
#include <Vector.h>
#include <SpinXML.h>

namespace spin
{
	class SnapConstraint: public ConstraintSpring 
	{
		public:
		SnapConstraint(): ConstraintSpring( 10, 10, 10 ), max_length( 20 ) {}
		SnapConstraint( float new_length, float new_strength, float new_damping, float new_max_length );

		void Render();
		void Tick( int milliseconds );

		virtual bool TryLoadElement( TiXmlElement* element, bool& error );
		virtual std::string GetXMLDesc() { return "SnapConstraint"; }

		friend class SpinXML;

		private:
		float max_length;
	};
}


#endif
