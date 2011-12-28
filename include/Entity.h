#ifndef SPIN_ENTITY_H
#define SPIN_ENTITY_H

#include <GL/gl.h>
#include <Vector.h>
#include <Color.h>
#include <Quad.h>
#include <string>
#include <vector>
#include <chipmunk.h>

struct cpBody;
struct cpShape;
struct cpSegmentShape;
struct cpConstraint;
struct TiXmlElement;

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
		int id;
		std::string alias;

		protected:
		Entity(): dead( false ), id( -1 ), alias( "UNNAMED" ) {}
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
		QuadEntity(): Entity(), rotation( 0.0 ), scale( 1.0 ) {}
		virtual ~QuadEntity() {}

		virtual void Render();

		Vector position;
		float rotation;
		float scale;

		void AddQuad( Quad new_quad ) { quads.push_back( new_quad ); }

		bool LoadXML( TiXmlElement* element );

		protected:
		std::vector<Quad> quads;
	};

	class ConstraintEntity: public Entity
	{
		public:
		ConstraintEntity();
		virtual ~ConstraintEntity();

		void Render();

		void ChipmunkCleanup();

		void InitConstraintSlideJoint( BodyEntity* new_body_a, BodyEntity* new_body_b, Vector offset_a, Vector offset_b, float min_length, float max_length );
		void InitConstraintSlideJoint( BodyEntity* new_body_a, Vector static_anchor, float min_length, float max_length );
		void InitConstraintSpring( BodyEntity* new_body_a, BodyEntity* new_body_b, Vector offset_a, Vector offset_b, float length, float strength, float damping );
		void InitConstraintSpring( BodyEntity* new_body_a, Vector static_anchor, float length, float strength, float damping );

		protected:

		cpConstraint* constraint;
	};
}

#endif
