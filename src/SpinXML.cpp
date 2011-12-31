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

bool SpinXML::ReadVec2D( TiXmlElement* element, std::string& name, Vector& vector_out )
{
	// check for null pointers
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
		fprintf( stderr, "SpinXML::ReadVec2D-> param missing either x or y attribute\n" );
		return false;
	}

	float new_x = 0.0;
	float new_y = 0.0;

	if( SpinUtil::ToFloat( x, new_x ) && SpinUtil::ToFloat( y, new_y ) )
	{
		name = (name_attribute)? name_attribute: "";
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

	name = ( name_att )? name_att: "";
	value = ( value_att )? value_att: "";
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
		*entity_out = new QuadEntity();
		QuadEntity* quad_entity = (QuadEntity*)*entity_out;
		if( quad_entity->LoadXML( element ) )
			return true;
		else
		{
			delete quad_entity;
			*entity_out = 0;
			fprintf( stderr, "SpinXML::ReadEntity -> ReadQuadEntity() failed!\n" );
			return false;
		}
	}
	// BodyEntity
	else if( strcmp( type, "body" ) == 0 )
	{
		*entity_out = new BodyEntity();
		BodyEntity* body_entity = (BodyEntity*)*entity_out;
		if( body_entity->LoadXML( element ) )
			return true;
		else
		{
			delete *entity_out;
			*entity_out = 0;
			fprintf( stderr, "SpinXML::ReadEntity -> BodyEntity::LoadXML() failed!\n" );
			return false;
		}
	}
	// static_body
	else if( strcmp( type, "static_body" ) == 0 )
	{
		*entity_out = SPIN.world.GetStaticBody();
		if( SPIN.world.GetStaticBody() != 0 && SPIN.world.GetStaticBody()->LoadXML( element ) )
			return true;
		else
		{
			fprintf( stderr, "SpinXML::ReadEntity -> StaticBody::LoadXML() failed!\n" );
			return false;
		}
	}
	// SnapConstraint
	if( strcmp( type, "snap_constraint" ) == 0 )
	{
		*entity_out = new SnapConstraint();
		SnapConstraint* snap_constraint = (SnapConstraint*)*entity_out;
		if( snap_constraint->LoadXML( element ) )
			return true;
		else
		{
			delete *entity_out;
			*entity_out = 0;
			fprintf( stderr, "SpinXML::ReadEntity -> SnapConstraint::LoadXML() failed!\n" );
			return false;
		}
	}
	else
	{
		fprintf( stderr, "SpinXML::ReadEntity -> unsupported entity type: '%s!'\n", type );
		return false;
	}
}
