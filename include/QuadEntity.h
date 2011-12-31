#ifndef SPIN_QUAD_ENTITY_H
#define SPIN_QUAD_ENTiTY_H

#include <Entity.h>

namespace spin
{
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
}

#endif
