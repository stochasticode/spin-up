#ifndef SPIN_QUAD_H
#define SPIN_QUAD_H

#include <Vector.h>
#include <SpinXML.h>
#include <Color.h>
#include <string>

struct TiXmlElement;

namespace spin
{
	class Quad: public SpinXML
	{
		public:
		std::string texture_key;
		Vector texture_top_left;
		Vector texture_top_right;
		Vector texture_bottom_left;
		Vector texture_bottom_right;

		Vector position;
		float rotation;
		Color color;

		enum QuadTextureMode { Q_TEXTURE_ABSOLUTE, Q_TEXTURE_RELATIVE };
		Quad();

		virtual void Render();

		void SetTextureModeRelative( Vector texture_scale, Vector texture_offset );
		void SetSize( Vector new_size );

		virtual bool TryLoadElement( TiXmlElement* element, bool& error );

		protected:
		QuadTextureMode texture_mode;
		Vector texture_scale;
		Vector texture_offset;
		Vector size;
	};
}

#endif
