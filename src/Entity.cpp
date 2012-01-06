#include <GL/gl.h>
#include <Entity.h>
#include <SpinGame.h>
#include <SpinXML.h>
#include <SpinUtil.h>
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
		// layer
		else if( name.compare( "layer" ) == 0 )
		{
			if( !SpinUtil::ToInt( value.c_str(), layer ) )
			{
				error = true;
				fprintf( stderr, "Entity::TryLoadElement -> invalid value for layer param!\n" );
				return false;
			}
		}
		else
			return false;
	}
	// unsupported
	else
		return false;
	
	return true;
}
