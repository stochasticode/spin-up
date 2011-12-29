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
		Quad();

		virtual void Render();

		bool LoadXML( TiXmlElement* element );

		std::string texture_key;
		Vector position;
		Vector size;
		float rotation;
		Color color;

		Vector texture_top_left;
		Vector texture_top_right;
		Vector texture_bottom_left;
		Vector texture_bottom_right;
	};
}

#endif
