#ifndef SPIN_BODY_ENTITY_H
#define SPIN_BODY_ENTITY_H

#include <QuadEntity.h>

struct TiXmlElement;
struct cpSpace;

namespace spin
{
	class BodyEntity: public QuadEntity
	{
		public:
		BodyEntity();
		virtual ~BodyEntity();

		// XML stuff
		virtual bool TryLoadElement( TiXmlElement* element, bool& error );
		virtual std::string GetXMLDesc() { return "BodyEntity"; }

		virtual void Render();
		virtual void Tick( int milliseconds );

		cpBody* GetBody() { return body; }

		float GetRotation() { return body->a; }
		void SetPosition( Vector new_position );
		void SetVelocity( Vector new_velocity );
		void SetMass( float mass );
		void SetMoment( float moment );
		void Scale( float scale_factor );

		static bool render_shapes;
		std::vector<unsigned long> constraint_ids;


		friend class World;

		protected:

		void RenderShapes();

		float MomentForCircle( float mass, float outer_radius, float inner_radius );
		void AddShapeCircle( float radius, Vector offset, float friction );
		void AddShapeRect( float width, float height, Vector offset, float friction );
		void AddShapePoly( std::vector<Vector>& points, float friction );
		void ChipmunkCleanup();

		bool LoadPolyElement( TiXmlElement* element );

		cpBody* body;
		std::vector<cpShape*> shapes;

		static GLfloat circle_points[];
	};

	class StaticBody: public BodyEntity
	{
		public:
		StaticBody();
		virtual ~StaticBody();
	};
}

#endif
