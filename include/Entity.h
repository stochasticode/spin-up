#ifndef SPIN_ENTITY_H
#define SPIN_ENTITY_H

#include <Vector.h>
#include <Color.h>
#include <string>
#include <vector>

struct cpBody;
struct cpShape;
struct cpSegmentShape;
struct cpConstraint;

namespace spin
{
	class BodyEntity;

	class Entity
	{
		public:
		virtual ~Entity() {}

		virtual void Tick( int milliseconds ) {}
		virtual void Render() {}

		bool dead;
		// this might not make much of a difference over just calling the methods...
		bool do_tick;
		bool do_render;
		int entity_id;

		protected:
		Entity(): dead( false ), do_tick( false ), do_render( false ) {}
	};

	class SurfaceEntity: public Entity
	{
		public:
		SurfaceEntity( float new_x1, float new_y1, float new_x2, float new_y2, float new_radius, float new_friction );
		~SurfaceEntity();

		virtual void Render();

		private:
		cpSegmentShape* shape;
		float radius;
	};

	class QuadEntity: public Entity
	{
		public:
		QuadEntity(): Entity(), scale( 1.0 ) {}
		virtual ~QuadEntity() {}

		virtual void Render();


		std::string texture_key;
		Vector position;
		Vector size;
		float rotation;
		Color color;
		float scale;
	};

	class ConstraintEntity: public Entity
	{
		public:
		virtual ~ConstraintEntity();

		void Render();

		void ChipmunkCleanup();

		void InitConstraintSlideJoint( BodyEntity* new_body_a, BodyEntity* new_body_b, float min_length, float max_length );
		void InitConstraintSlideJoint( BodyEntity* new_body_a, Vector static_anchor, float min_length, float max_length );
		void InitConstraintSpring( BodyEntity* new_body_a, BodyEntity* new_body_b, float length, float strength, float damping );
		void InitConstraintSpring( BodyEntity* new_body_a, Vector static_anchor, float length, float strength, float damping );

		protected:
		ConstraintEntity();

		cpConstraint* constraint;
	};

	class BodyEntity: public QuadEntity
	{
		public:
		virtual ~BodyEntity();

		void SetPosition( Vector new_position );
		void SetVelocity( Vector new_velocity );

		friend class ConstraintEntity;

		std::vector<unsigned long> constraint_ids;

		protected:
		BodyEntity();

		virtual void Tick( int milliseconds );

		void ChipmunkCleanup();
		void InitBodyCircle( float mass, float radius, float friction );
		void InitBodyRect( float mass, float width, float height, float friction );

		//void RemoveConstraint( ConstraintEntity* constraint_removed );

		cpBody *body;
		cpShape *shape;
	};
}

#endif
