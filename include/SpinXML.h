#ifndef SPIN_XML_H
#define SPIN_XML_H

#include <string>

struct TiXmlElement;

namespace spin
{
	class Vector;
	class Color;
	
	class SpinXML
	{
		public:
		bool LoadXML( const char* xml_path );
		virtual bool LoadElements( TiXmlElement* element );
		virtual void FinalizeLoadElements() {}
		virtual std::string GetXMLDesc() { return "SpinXML"; }

		protected:
		virtual bool TryLoadElement( TiXmlElement* element, bool& error ) = 0;
		bool ReadVec2D( TiXmlElement* element, std::string& name, Vector& vector );
		bool ReadParam( TiXmlElement* element, std::string& name, std::string& value );
		bool ReadColor( TiXmlElement* element, std::string& name, Color& color );
	};
}

#endif
