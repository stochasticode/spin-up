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
		virtual void FinalizeLoadElements();
		virtual std::string GetXMLDesc() { return "BodyEntity"; }

		virtual void Render();
		virtual void Tick( int milliseconds );

		void SetLayers( int layers );

		cpBody* GetBody() { return body; }

		float GetRotation() { return body->a; }
		virtual void SetPosition( Vector new_position );
		void SetVelocity( Vector new_velocity );
		void ApplyImpulse( Vector new_velocity );
		void ApplyForce( Vector new_velocity );
		void SetMass( float mass );
		void SetMoment( float moment );
		void Scale( float scale_factor );

		static bool render_shapes;
		std::vector<unsigned long> constraint_ids;

		void AddShapeCircle( float radius, Vector offset, float friction );
		void AddShapeRect( float width, float height, Vector offset, float friction );
		void AddShapePoly( std::vector<Vector>& points, float friction );

		protected:

		void RenderShapes();

		static int lua_AddImpulse( lua_State *L );

		float MomentForCircle( float mass, float outer_radius, float inner_radius );
		void ChipmunkCleanup();

		bool LoadPolyElement( TiXmlElement* element );

		cpBody* body;
		std::vector<cpShape*> shapes;

		static GLfloat circle_points[];

		friend class World;
	};

	class StaticBody: public BodyEntity
	{
		public:
		StaticBody();
		virtual ~StaticBody();
	};
}

#endif
