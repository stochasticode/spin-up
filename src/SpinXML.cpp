#include <SpinGame.h>
#include <SpinXML.h>
#include <SpinUtil.h>
#include <Entity.h>
#include <BodyEntity.h>
#include <SnapConstraint.h>
#include <Vector.h>
#include <tinyxml.h>
#include <string>

using namespace spin;

bool SpinXML::LoadXML( const char* xml_path )
{
	// load document
	TiXmlDocument doc( xml_path );
	if( !doc.LoadFile() )
	{
		fprintf( stderr, "SpinXML::LoadXML -> unable to load entity xml: %s\n", xml_path );
		return false;
	}

	// find root
	TiXmlElement* root = doc.FirstChildElement();
	if( !root )
	{
		fprintf( stderr, "SpinXML::LoadXML -> unable to find root tag in xml: %s\n", xml_path );
		return false;
	}

	return LoadElements( root );
}

bool SpinXML::LoadElements( TiXmlElement* element )
{
	// check for NULL
	if( element == 0 )
	{
		fprintf( stderr, "SpinXML::LoadElement -> element was NULL!\n" );
		return false;
	}

	TiXmlElement* child = element->FirstChildElement();
	while( child != 0 )
	{
		// try and load element
		bool error = false;
		if( !TryLoadElement( child, error ) )
		{
			if( error )
				return false;
			else
				fprintf( stderr, "SpinXML::LoadElement -> (%s) unsupported element '%s', name='%s'\n", GetXMLDesc().c_str(), child->Value(), child->Attribute( "name" ) );
		}
		child = child->NextSiblingElement();
	}
	FinalizeLoadElements();
	return true;
}

bool SpinXML::ReadVec2D( TiXmlElement* element, std::string& name, Vector& vector )
{
	if( element == 0 )
	{
		fprintf( stderr, "SpinXML::ReadVec2D -> element was NULL!\n" );
		return false;
	}

	const char* name_attribute = element->Attribute( "name" );
	const char* x = element->Attribute( "x" );
	const char* y = element->Attribute( "y" );

	if( !x || !y )
	{
		fprintf( stderr, "SpinXML::ReadVec2D-> vec2d missing either x or y attribute\n" );
		return false;
	}

	float new_x = 0.0;
	float new_y = 0.0;

	if( SpinUtil::ToFloat( x, new_x ) && SpinUtil::ToFloat( y, new_y ) )
	{
		name = (name_attribute)? name_attribute: "";
		vector.x = new_x;
		vector.y = new_y;
		return true;
	}
	else
	{
		fprintf( stderr, "SpinXML::ReadVec2D-> invalid x or y attribute\n" );
		return false;
	}
}

bool SpinXML::ReadParam( TiXmlElement* element, std::string& name, std::string& value )
{
	// check for null pointers
	if( element == 0 )
	{
		fprintf( stderr, "SpinXML::ReadParam -> element was NULL!\n" );
		return false;
	}

	const char* name_att = element->Attribute( "name" );
	const char* value_att = element->Attribute( "value" );

	if( !name_att || !value_att )
	{
		fprintf( stderr, "SpinXML::ReadParam -> param missing either name or value attribute\n" );
		return false;
	}

	name = name_att;
	value = value_att;
	return true;
}

bool SpinXML::ReadColor( TiXmlElement* element, std::string& name, Color& color )
{
	// check for null pointers
	if( element == 0 )
	{
		fprintf( stderr, "SpinXML::ReadColor -> element was NULL!\n" );
		return false;
	}

	const char* name_att = element->Attribute( "name" );
	const char* r_att = element->Attribute( "r" );
	const char* g_att = element->Attribute( "g" );
	const char* b_att = element->Attribute( "b" );
	const char* a_att = element->Attribute( "a" );

	if( !r_att || !g_att || !b_att || !a_att )
	{
		fprintf( stderr, "SpinXML::ReadColor-> color missing r, g, b, or a attribute\n" );
		return false;
	}

	float r = 1.0;
	float g = 1.0;
	float b = 1.0;
	float a = 1.0;

	if( SpinUtil::ToFloat( r_att, r ) && SpinUtil::ToFloat( g_att, g ) && SpinUtil::ToFloat( b_att, b ) && SpinUtil::ToFloat( a_att, a ) )
	{
		name = (name_att)? name_att: "";
		color.r = r;	
		color.g = g;	
		color.b = b;	
		color.a = a;	
		return true;
	}
	else
	{
		fprintf( stderr, "SpinXML::ReadColor -> invalid r, g, b, or a attribute\n" );
		return false;
	}
}
