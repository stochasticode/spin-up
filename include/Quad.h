#ifndef SPIN_QUAD_H
#define SPIN_QUAD_H

#include <Vector.h>
#include <Color.h>
#include <string>

struct TiXmlElement;

namespace spin
{
	class Quad
	{
		public:
		Quad(): texture_key( "DEFAULT" ), position( 0.0, 0.0 ), size( 10.0, 10.0 ), rotation( 0.0 ) {}

		virtual void Render();

		bool LoadXML( TiXmlElement* element );

		std::string texture_key;
		Vector position;
		Vector size;
		float rotation;
		Color color;
	};
}

#endif
