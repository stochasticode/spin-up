#ifndef GAL_QUAD_H
#define GAL_QUAD_H

#include <Vector.h>
#include <Color.h>
#include <string>

namespace spin
{ 
	class Quad
	{
		public:
		Quad(): scale( 1.0 )  {}

		virtual void Render();

		std::string texture_key;
		Vector position;
		Vector size;
		float rotation;
		Color color;
		float scale;
	};
}

#endif
