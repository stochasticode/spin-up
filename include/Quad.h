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
		enum QuadTextureMode { Q_TEXTURE_ABSOLUTE, Q_TEXTURE_RELATIVE };
		Quad();

		virtual void Render();

		bool LoadXML( TiXmlElement* element );

		void SetTextureModeRelative( Vector texture_scale, Vector texture_offset );
		void SetSize( Vector new_size );

		std::string texture_key;
		Vector texture_top_left;
		Vector texture_top_right;
		Vector texture_bottom_left;
		Vector texture_bottom_right;

		Vector position;
		float rotation;
		Color color;

		protected:
		QuadTextureMode texture_mode;
		Vector texture_scale;
		Vector texture_offset;
		Vector size;
	};
}

#endif
