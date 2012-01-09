#ifndef SPIN_QUAD_ENTITY_H
#define SPIN_QUAD_ENTITY_H

#include <Entity.h>
#include <stdio.h>

namespace spin
{
	class QuadEntity: public Entity
	{
		public:
		Vector position;
		float rotation;
		float scale;

		QuadEntity();
		virtual ~QuadEntity() {}

		virtual void SetPosition( Vector new_position ) { 
			position = new_position;
			printf( "quad set\n" );
			fflush( stdout );
		}

		virtual void Render();
		void AddQuad( Quad new_quad ) { quads.push_back( new_quad ); }

		void SetColorAll( Color new_color );

		virtual std::string GetXMLDesc() { return "QuadEntity"; }

		protected:
		std::vector<Quad> quads;

		virtual bool TryLoadElement( TiXmlElement* element, bool& error );
	};
}

#endif
