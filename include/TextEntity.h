#ifndef SPIN_TEXT_ENTITY_H
#define SPIN_TEXT_ENTITY_H

#include <QuadEntity.h>

namespace spin
{
	class TextEntity: public QuadEntity
	{
		public:
		void SetText( const char* new_text );

		virtual bool TryLoadElement( TiXmlElement* element, bool& error );
		virtual std::string GetXMLDesc() { return "TextEntity"; }

		private:
		void AddLetter( char character, int index );
	};
}

#endif
