#ifndef SPIN_GRAPPLE_GUN_H
#define SPIN_GRAPPLE_GUN_H

#include <BodyEntity.h>
#include <ConstraintEntity.h>
#include <Vector.h>
#include <Color.h>
#include <vector>

#ifndef MAX_GRAPPLE_HOOKS
	#define MAX_GRAPPLE_HOOKS 5
#endif

struct cpArbiter;
struct cpSpace;

namespace spin
{
	class GrappleGun;

	enum GrappleConstraintType { GRAPPLE_WINCH, GRAPPLE_SPRINGY };

	class GrappleInfo
	{
		public:
		GrappleInfo(): hook_index( 0 ), hook_id( 0 ), constraint_id( 0 ), type( GRAPPLE_SPRINGY ), parent_gun( 0 ) {}
		int hook_index;
		unsigned long hook_id;
		unsigned long constraint_id;
		GrappleConstraintType type;
		GrappleGun* parent_gun;
		Color color;
	};

	class GrappleHook: public BodyEntity
	{
		public:
		GrappleHook( int new_life_left, GrappleGun* new_parent_gun, GrappleInfo new_info );

		int life_left;
		GrappleInfo info;

		void Render();
		void Tick( int milliseconds );
	};

	class GrappleConstraint
	{
		public:
		void Attach( BodyEntity* body_a, BodyEntity* body_b, Vector offset_a, Vector offset_b );

		protected:
		GrappleConstraint( GrappleInfo new_info ): info( new_info ) {}
		GrappleInfo info;

		void Render( cpConstraint* own_constraint );
	};

	class GrappleConstraintWinch: public GrappleConstraint, public ConstraintSlideJoint
	{
		public:
		GrappleConstraintWinch( GrappleInfo new_info ): GrappleConstraint( new_info ) {}

		void Render();
	};

	class GrappleConstraintSpringy: public GrappleConstraint, public ConstraintSpring
	{
		public:
		GrappleConstraintSpringy( GrappleInfo new_info ): GrappleConstraint( new_info ) {};

		void Render();
	};

	class GrappleGun
	{
		public:
		GrappleGun();

		void SetGrappleType( int hook_index, GrappleConstraintType type );
		void SwitchHook( int hook_index );
		void ActivateHook( Vector position, Vector direction );
		void DeactivateHook();
		void DeactivateHook( int hook_index );
		void DeactivateAllHooks();
		static void PostSolveGrapple( cpArbiter *arb, cpSpace *space, void *unused );
		static void PostStepGrapple( cpSpace* space, cpShape* shape, cpShape* prop_shape );

		int GetCurrentHook() { return current_hook; }

		private:
		int current_hook;
		GrappleInfo grapple_info[MAX_GRAPPLE_HOOKS];
	};
};

#endif
