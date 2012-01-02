#ifndef SPIN_XML_H
#define SPIN_XML_H

#include <string>

struct TiXmlElement;

namespace spin
{
	class Vector;
	
	class SpinXML
	{
		public:
		bool LoadXML( const char* xml_path );
		bool LoadElements( TiXmlElement* element );

		protected:
		virtual bool TryLoadElement( TiXmlElement* element, bool& error ) = 0;
		bool ReadVec2D( TiXmlElement* element, std::string& name, Vector vector );
		bool ReadParam( TiXmlElement* element, std::string& name, std::string& value );
	};
}

#endif
