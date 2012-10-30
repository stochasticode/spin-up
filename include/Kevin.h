#ifndef SPIN_KEVIN_H
#define SPIN_KEVIN_H

#include <BodyEntity.h>
#include <Controls.h>

namespace spin
{
	class Kevin: public BodyEntity, public Controls
	{
		public:
		Kevin();
		~Kevin();

		protected:
		virtual void ProcessControlEvent( ControlEvent event );
		virtual void ProcessRawKeyEvent( RawKeyEvent event );

		virtual void Tick( int milliseconds );

		private:
		cpShape* foot_shape;
		bool running_left;
		bool running_right;
		bool grounded;
		float run_velocity;

		static void SelectPlayerGroundNormal( cpBody *body, cpArbiter *arb, cpVect *ground_normal );
	};
}
#endif
