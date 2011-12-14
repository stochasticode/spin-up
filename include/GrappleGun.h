#ifndef SPIN_GRAPPLE_GUN_H
#define SPIN_GRAPPLE_GUN_H

#include <Entity.h>
#include <Vector.h>
#include <vector>

namespace spin
{
	class GrappleHook: public BodyEntity
	{
		public:
		GrappleHook( int new_life_left );

		int life_left;

		void Render();
		void Tick( int milliseconds );
	};

	class GrappleGun
	{
		public:
		GrappleGun( int new_max_hooks );

		void ActivateHook( Vector position, Vector direction );
		void DeactivateHook();

		// hooks are Entities so they render themselves but we need to render the "ropes"
		void Render();

		private:
		int current_hook;
		int max_hooks;
		std::vector<unsigned long> hooks;
	};
};

#endif
