#ifndef SPIN_ENTITY_H
#define SPIN_ENTITY_H

#include <GL/gl.h>
#include <Vector.h>
#include <Color.h>
#include <Quad.h>
#include <SpinXML.h>
#include <string>
#include <vector>
#include <chipmunk.h>

struct TiXmlElement;

namespace spin
{
	class Entity: public SpinXML
	{
		public:
		bool dead;
		std::string alias;
		int layer;

		virtual ~Entity() {}

		virtual void SetID( int new_id ) { id = new_id; }
		int GetID() { return id; }

		virtual void Tick( int milliseconds ) {}
		virtual void Render() {}

		virtual std::string GetXMLDesc() { return "Entity"; }

		protected:
		int id;

		Entity(): dead( false ), id( -1 ), alias( "UNNAMED" ), layer( 4 ) {}

		virtual bool TryLoadElement( TiXmlElement* element, bool& error );
	};
}

#endif
