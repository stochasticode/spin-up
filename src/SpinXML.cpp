#include <SpinXML.h>
#include <SpinUtil.h>
#include <Entity.h>
#include <Vector.h>
#include <tinyxml.h>
#include <string>

using namespace spin;

bool SpinXML::ReadVec2D( TiXmlElement* element, Vector& vector_out )
{
	// check for null pointers
	if( element == 0 )
	{
		fprintf( stderr, "SpinXML::ReadVec2D -> element was NULL!\n" );
		return false;
	}

	const char* x = element->Attribute( "x" );
	const char* y = element->Attribute( "y" );

	if( !x || !y )
	{
		fprintf( stderr, "SpinXML::ReadVec2D-> param missing either x or y attribute\n" );
		return false;
	}

	float new_x = 0.0;
	float new_y = 0.0;

	if( SpinUtil::ToFloat( x, new_x ) && SpinUtil::ToFloat( y, new_y ) )
	{
		vector_out.x = new_x;
		vector_out.y = new_y;
		return true;
	}
	else
	{
		fprintf( stderr, "SpinXML::ReadVec2D-> invalid x or y attribute\n" );
		return false;
	}
}

bool SpinXML::ReadParam( TiXmlElement* element, std::string& value )
{
	// check for null pointers
	if( element == 0 )
	{
		fprintf( stderr, "SpinXML::ReadParam -> element was NULL!\n" );
		return false;
	}

	const char* param_value = element->Attribute( "value" );

	if( !param_value )
	{
		fprintf( stderr, "SpinXML::ReadParam -> param missing value attribute\n" );
		return false;
	}

	value = param_value;
	return true;
}

bool SpinXML::ReadEntity( TiXmlElement* element, Entity** entity_out  )
{
	// check for null pointers
	if( element == 0 || entity_out == 0 )
	{
		fprintf( stderr, "SpinXML::ReadEntity -> either element or entity_out was NULL!\n" );
		return false;
	}

	// get entity type
	const char* type = element->Attribute( "type" );
	if( type == 0 )
	{
		fprintf( stderr, "SpinXML::ReadEntity -> no type attribute!\n" );
		return false;
	}

	if( strcmp( type, "quad" ) == 0 )
	{
		QuadEntity* new_quad_entity;
		if( ReadQuadEntity( element, &new_quad_entity ) )
		{
			*entity_out = new_quad_entity;
			return true;
		}
		else
		{
			fprintf( stderr, "SpinXML::ReadEntity -> ReadQuadEntity() failed!\n" );
			return false;
		}
	}
	else
	{
		fprintf( stderr, "SpinXML::ReadEntity -> unsupported entity type: '%s!'\n", type );
		return false;
	}
}

bool SpinXML::ReadQuadEntity( TiXmlElement* element, QuadEntity** entity_out )
{
	Vector size;
	Vector position;
	std::string texture_key = "default";

	TiXmlElement* child = element->FirstChildElement();
	while( child != 0 )
	{
		// Vector
		if( strcmp( "vec2d", child->Value() ) == 0 )
		{
			// size
			if( strcmp( "size", child->Attribute( "name" ) ) == 0 )
			{
				if( !ReadVec2D( child, size ) )
				{
					fprintf( stderr, "SpinXML::ReadQuadEntity -> ReadVec2D failed for size vec2d!\n" );
					return false;
				}
			}
			// position 
			else if( strcmp( "position", child->Attribute( "name" ) ) == 0 )
			{
				if( !ReadVec2D( child, position ) )
				{
					fprintf( stderr, "SpinXML::ReadQuadEntity -> ReadVec2D failed for position vec2d!\n" );
					return false;
				}
			}
			// unsupported vec2d
			else
				fprintf( stderr, "SpinXML::ReadQuadEntity -> unsupported vec2d, name='%s'...\n", child->Attribute( "name" ) );
		}
		// param
		else if( strcmp( "param", child->Value() ) == 0 )
		{
			// texture_key
			if( strcmp( "texture_key", child->Attribute( "name" ) ) == 0 )
			{
				if( !ReadParam( child, texture_key ) )
				{
					fprintf( stderr, "SpinXML::ReadQuadEntity -> ReadParam failed for texture_key param!\n" );
					return false;
				}
			}
			// unsupported param
			else
				fprintf( stderr, "SpinXML::ReadQuadEntity -> unsupported param, name='%s'...\n", child->Attribute( "name" ) );
		}
		child = child->NextSiblingElement();
	}

	*entity_out = new QuadEntity();
	(*entity_out)->size = size;
	(*entity_out)->position = position;
	(*entity_out)->texture_key = texture_key;

	printf( "size: %f %f\n", size.x, size.y );
	printf( "position: %f %f\n", position.x, position.y );
	printf( "texture_key: %s\n", texture_key.c_str() );
	fflush( stdout );

	return true;
}
