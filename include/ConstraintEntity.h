#ifndef SPIN_CONSTRAINT_ENTITY_H
#define SPIN_CONSTRAINT_ENTITY_H

#include <Entity.h>

namespace spin
{
	class BodyEntity;

	class ConstraintEntity: public Entity
	{
		public:
		virtual ~ConstraintEntity();

		virtual void Attach( BodyEntity* body_a, BodyEntity* body_b, Vector offset_a, Vector offset_b ) = 0;

		virtual std::string GetXMLDesc() { return "ConstraintEntity"; }

		protected:
		ConstraintEntity();

		cpConstraint* constraint;

		void ChipmunkCleanup();
	};

	class ConstraintSlideJoint: public ConstraintEntity
	{
		public:
		ConstraintSlideJoint( float new_min_length, float new_max_length ): min_length( new_min_length ), max_length( new_max_length )  {}
		virtual void Attach( BodyEntity* body_a, BodyEntity* body_b, Vector offset_a, Vector offset_b );

		float min_length;
		float max_length;

		protected:
		ConstraintSlideJoint(): min_length( 0 ), max_length( 10 ) {}
	};

	class ConstraintSpring: public ConstraintEntity
	{
		public:
		ConstraintSpring( float new_length, float new_strength, float new_damping ): length( new_length ), strength( new_strength ), damping( new_damping ) {}
		virtual void Attach( BodyEntity* body_a, BodyEntity* body_b, Vector offset_a, Vector offset_b );

		float length;
		float strength;
		float damping;

		protected:
		ConstraintSpring(): length( 10 ), strength( 10 ), damping( 10 ) {}
	};
}

#endif
