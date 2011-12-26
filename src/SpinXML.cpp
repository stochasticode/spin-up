#include <SpinGame.h>
#include <SpinXML.h>
#include <SpinUtil.h>
#include <Entity.h>
#include <SnapConstraint.h>
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

	// QuadEntity
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

bool SpinXML::ReadConstraint( TiXmlElement* element, ConstraintEntity** constraint_out  )
{
	// check for null pointers
	if( element == 0 || constraint_out == 0 )
	{
		fprintf( stderr, "SpinXML::ReadConstraintEntity -> either element or constraint_out was NULL!\n" );
		return false;
	}

	// get entity type
	const char* type = element->Attribute( "type" );
	if( type == 0 )
	{
		fprintf( stderr, "SpinXML::ReadConstraintEntity -> no type attribute!\n" );
		return false;
	}

	// SnapConstraint
	if( strcmp( type, "snap" ) == 0 )
	{
		SnapConstraint* new_quad_entity;
		if( ReadSnapConstraint( element, &new_quad_entity ) )
		{
			*constraint_out = new_quad_entity;
			return true;
		}
		else
		{
			fprintf( stderr, "SpinXML::ReadConstraint -> ReadSnapConstraint() failed!\n" );
			return false;
		}
	}
	else
	{
		fprintf( stderr, "SpinXML::ReadConstraint -> unsupported constraint type: '%s!'\n", type );
		return false;
	}
}

bool SpinXML::ReadSnapConstraint( TiXmlElement* element, SnapConstraint** constraint_out )
{
	// first child must be an entity_ref to a BodyEntity
	Entity* entity_a = 0;
	TiXmlElement* child1 = element->FirstChildElement();
	if( child1 == 0 )
	{
		fprintf( stderr, "SpinXML::ReadSnapConstraint -> constraint has no child elements!'\n" );
		return false;
	}
	if( strcmp( "entity_ref", child1->Value() ) != 0 )
	{
		fprintf( stderr, "SpinXML::ReadSnapConstraint -> first child was not an entity_ref!'\n" );
		return false;
	}
	std::string alias1( child1->Attribute( "entity_alias" ) );
	entity_a = SPIN.world.GetEntityByAlias( alias1 );
	if( !entity_a )
	{
		fprintf( stderr, "SpinXML::ReadSnapConstraint -> could not find entity with alias: %s!'\n", alias1.c_str() );
		return false;
	}
	// make sure it is a BodyEntity
	BodyEntity* body_a = dynamic_cast<BodyEntity*>(entity_a);
	if( body_a == 0 )
	{
		fprintf( stderr, "SpinXML::ReadSnapConstraint -> entity with alias: %s is not a BodyEntity!'\n", alias1.c_str() );
		return false;
	}

	// for now second child needs to be a vec2d
	Vector static_anchor;
	TiXmlElement* child2 = child1->NextSiblingElement();
	if( child2 == 0 )
	{
		fprintf( stderr, "SpinXML::ReadSnapConstraint -> constraint has only one child element!'\n" );
		return false;
	}
	if( strcmp( "vec2d", child2->Value() ) != 0 || !ReadVec2D( child2, static_anchor) )
	{
		fprintf( stderr, "SpinXML::ReadSnapConstraint -> ReadVec2D failed for second child element!'\n" );
		return false;
	}

	*constraint_out = new SnapConstraint( body_a, static_anchor );
	return true;
}
