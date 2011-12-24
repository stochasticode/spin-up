#ifndef SPIN_ENTITY_H
#define SPIN_ENTITY_H

#include <GL/gl.h>
#include <Vector.h>
#include <Color.h>
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
		int entity_id;

		protected:
		Entity(): dead( false ) {}
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
		QuadEntity(): Entity(), size( 10.0, 10.0 ), rotation( 0.0 ), scale( 1.0 ) {}
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

	class BodyEntity: public QuadEntity
	{
		public:
		virtual ~BodyEntity();

		virtual void Render();

		static BodyEntity* LoadEntity( const char* xml_path );

		float GetRotation() { return ( body != 0 )? body->a: 0.0; }
		void SetPosition( Vector new_position );
		void SetVelocity( Vector new_velocity );

		void Scale( float scale_factor );

		friend class ConstraintEntity;

		std::vector<unsigned long> constraint_ids;

		static bool render_shapes;

		protected:
		BodyEntity();

		bool LoadXML( const char* xml_path );

		bool LoadPolyElement( TiXmlElement* element, float mass, float friction );

		virtual void Tick( int milliseconds );

		void ChipmunkCleanup();
		void InitBodyCircle( float mass, float radius, float friction );
		void InitBodyRect( float mass, float width, float height, float friction );
		void InitBodyPoly( float mass, std::vector<Vector>& points, float friction );

		void RenderShape();

		cpBody *body;
		cpShape *shape;

		static GLfloat circle_points[];
	};
}

#endif
