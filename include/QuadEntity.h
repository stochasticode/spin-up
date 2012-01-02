#ifndef SPIN_QUAD_ENTITY_H
#define SPIN_QUAD_ENTiTY_H

#include <Entity.h>

namespace spin
{
	class QuadEntity: public Entity
	{
		public:
		Vector position;
		float rotation;
		float scale;

		QuadEntity(): Entity(), rotation( 0.0 ), scale( 1.0 ) {}
		virtual ~QuadEntity() {}

		virtual void Render();
		void AddQuad( Quad new_quad ) { quads.push_back( new_quad ); }

		protected:
		std::vector<Quad> quads;

		virtual bool TryLoadElement( TiXmlElement* element, bool& error );
	};
}

#endif
