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
struct lua_State;

namespace spin
{
	class Entity: public SpinXML
	{
		public:
		bool dead;
		std::string alias;
		int layer;
		std::string master_script_text;
		std::string script_text;

		virtual ~Entity();

		virtual void SetID( int new_id );
		int GetID() { return id; }

		virtual void Tick( int milliseconds );
		virtual void Render() {}
		std::string GetScriptText() { return script_text; }

		virtual std::string GetXMLDesc() { return "Entity"; }

		bool LoadScript( const char* script, bool master );
		bool LoadScriptString( const char* script, bool master );

		protected:
		int id;
		lua_State* lua_state;

		Entity();

		virtual bool TryLoadElement( TiXmlElement* element, bool& error );
	};
}

#endif
