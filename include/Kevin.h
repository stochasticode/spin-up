#ifndef SPIN_KEVIN_H
#define SPIN_KEVIN_H

#include <Entity.h>
#include <GrappleGun.h>
//#include <Vector.h>

namespace spin
{
	class Kevin: public BodyEntity
	{
		public:
		Kevin();
		~Kevin();

		void Render();
		void Tick( int milliseconds );

		//void EnableGrapple( float x, float y );
		//void DisableGrapple();

		void FirePrimary( Vector direction );
		void FireSecondary( Vector direction );

		private:
		GrappleGun grapple_gun;
	};
}
#endif
