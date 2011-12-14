#ifndef SPIN_GRAPPLE_GUN_H
#define SPIN_GRAPPLE_GUN_H

#include <Entity.h>
#include <vector>

namespace spin
{
	class GrappleHook: public BodyEntity
	{
		public:
		GrappleHook( int new_life_left );

		int life_left;

		void Tick( int milliseconds );
	};

	class GrappleGun
	{
		public:
		GrappleGun( int new_max_hooks );

		void ActivateHook( int hook_index );
		void DeactivateHook( int hook_index );

		// hooks are Entities so they render themselves but we need to render the "ropes"
		void Render();
		void Tick( int milliseconds );

		private:
		int max_hooks;
		std::vector<GrappleHook*> hooks;
	};
};

#endif
