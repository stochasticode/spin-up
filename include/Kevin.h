#ifndef SPIN_KEVIN_H
#define SPIN_KEVIN_H

#include <Entity.h>
#include <GrappleGun.h>

namespace spin
{
	class Kevin: public BodyEntity
	{
		public:
		Kevin();
		~Kevin();

		void FirePrimary( Vector direction );
		void FireSecondary( Vector direction );

		GrappleGun grapple_gun;
	};
}
#endif
