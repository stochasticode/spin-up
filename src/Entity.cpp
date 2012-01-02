#include <GL/gl.h>
#include <Entity.h>
#include <SpinGame.h>
#include <SpinXML.h>
#include <chipmunk.h>
#include <tinyxml.h>
#include <sstream>
#include <cstdio>

using namespace spin;

bool Entity::TryLoadElement( TiXmlElement* element, bool& error )
{
	error = false;

	// check for NULL
	if( element == 0 )
	{
		fprintf( stderr, "Entity::TryLoadElement -> element was NULL!\n" );
		error = true;
		return false;
	}

	// param
	if( strcmp( "param", element->Value() ) == 0 )
	{
		std::string name = "";
		std::string value = "";
		if( !ReadParam( element, name, value ) )
		{
			error = true;
			fprintf( stderr, "Entity::TryLoadElement -> ReadParam failed!\n" );
			return false;
		}

		// alias
		if( name.compare( "alias" ) == 0 )
			alias = value;
		else
			return false;
	}
	// unsupported
	else
		return false;
	
	return true;
}
