#ifndef SPIN_GRAPPLE_GUN_H
#define SPIN_GRAPPLE_GUN_H

#include <Entity.h>
#include <Vector.h>
#include <vector>

struct cpArbiter;
struct cpSpace;

namespace spin
{
	class GrappleGun;

	class GrappleHook: public BodyEntity
	{
		public:
		GrappleHook( int new_life_left, GrappleGun* new_parent_gun );

		int life_left;
		GrappleGun* parent_gun;

		void Render();
		void Tick( int milliseconds );
	};

	class GrappleGun
	{
		public:
		GrappleGun( int new_max_hooks );

		void ActivateHook( Vector position, Vector direction );
		void DeactivateHook();
		static void PostSolveGrapple( cpArbiter *arb, cpSpace *space, void *unused );
		static void PostStepGrapple( cpSpace* space, cpShape* shape, void* unused );

		// hooks are Entities so they render themselves but we need to render the "ropes"
		void Render();

		private:
		int current_hook;
		int max_hooks;
		std::vector<unsigned long> hooks;
		std::vector<unsigned long> constraints;
	};
};

#endif
