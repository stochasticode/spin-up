#include <ConstraintEntity.h>
#include <SpinGame.h>
#include <SpinUtil.h>
#include <chipmunk.h>
#include <cstdio>
#include <tinyxml.h>

using namespace spin;

ConstraintEntity::ConstraintEntity(): constraint( 0 )
{
}

ConstraintEntity::~ConstraintEntity()
{
	ChipmunkCleanup();
}

void ConstraintEntity::ChipmunkCleanup()
{
	if( constraint != 0 )
	{
		cpSpaceRemoveConstraint( SPIN.world.GetCPSpace(), constraint );
		cpConstraintFree( constraint );
		constraint = 0;
	}
}

void ConstraintSlideJoint::Attach( BodyEntity* new_body_a, BodyEntity* new_body_b, Vector offset_a, Vector offset_b )
{
	ChipmunkCleanup();
	if( new_body_a != 0 && new_body_a->GetBody() != 0 && new_body_b != 0 && new_body_b->GetBody() != 0 )
	{
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpSlideJointNew( new_body_a->GetBody(), new_body_b->GetBody(), cpv( offset_a.x, offset_a.y ), cpv( offset_b.x, offset_b.y ), min_length, max_length ) );
		new_body_a->constraint_ids.push_back( id );
		new_body_b->constraint_ids.push_back( id );
	}
	else
		fprintf( stderr, "ConstraintSlideJoint::Attach -> failed due to null pointer!\n" );
}

// spring between two bodies
void ConstraintSpring::Attach( BodyEntity* new_body_a, BodyEntity* new_body_b, Vector offset_a, Vector offset_b )
{
	ChipmunkCleanup();
	if( new_body_a != 0 && new_body_a->GetBody()!= 0 && new_body_b != 0 && new_body_b->GetBody()!= 0 )
	{
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpDampedSpringNew( new_body_a->GetBody(), new_body_b->GetBody(), cpv( offset_a.x, offset_a.y ), cpv( offset_b.x, offset_b.y ), length, strength, damping ) );
		new_body_a->constraint_ids.push_back( id );
		new_body_b->constraint_ids.push_back( id );
	}
	else
		fprintf( stderr, "ConstraintSpring::Attach -> failed due to null pointer!\n" );
}

bool ConstraintEntity::TryLoadElement( TiXmlElement* element, bool& error )
{
	error = false;

	// check for NULL
	if( element == 0 )
	{
		error = true;
		fprintf( stderr, "ConstraintEntity::TryLoadElement -> element was NULL!\n" );
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
			fprintf( stderr, "ConstraintEntity::TryLoadElement -> ReadParam failed!\n" );
			return false;
		}

		// body_a_id 
		if( name.compare( "body_a_alias" ) == 0 )
		{
			Entity* body_a = SPIN.world.GetEntityByAlias( value );
			if( body_a == 0 )
			{
				error = true;
				fprintf( stderr, "ConstraintEntity::TryLoadElement -> no entity with alias '%s' found for body_a!\n", value.c_str() );
				return false;
			}
			body_a_id = body_a->GetID();
		}
		else if( name.compare( "body_b_alias" ) == 0 )
		{
			Entity* body_b = SPIN.world.GetEntityByAlias( value );
			if( body_b == 0 )
			{
				error = true;
				fprintf( stderr, "ConstraintEntity::TryLoadElement -> no entity with alias '%s' foud for body_b!\n", value.c_str() );
				return false;
			}
			body_b_id = body_b->GetID();
		}
		else
			return Entity::TryLoadElement( element, error );
	}
	// vec2d
	else if( strcmp( "vec2d", element->Value() ) == 0 )
	{
		std::string name = "";
		Vector vec2d;
		if( !SpinXML::ReadVec2D( element, name, vec2d ) )
		{
			error = true;
			fprintf( stderr, "ConstraintEntity::TryLoadElement -> ReadVec2D failed!\n" );
			return false;
		}

		// body_a_offset
		if( name.compare( "body_a_offset" ) == 0 )
			offset_a = vec2d;
		// body_b_offset
		else if( name.compare( "body_b_offset" ) == 0 )
			offset_b = vec2d;
		// unsupported
		else
			return Entity::TryLoadElement( element, error );
	}
	// unsupported
	else
		return Entity::TryLoadElement( element, error );
	
	return true;
}

void ConstraintEntity::FinalizeLoadElements()
{
	BodyEntity* body_a = dynamic_cast<BodyEntity*>(SPIN.world.GetEntity( body_a_id ));
	BodyEntity* body_b = dynamic_cast<BodyEntity*>(SPIN.world.GetEntity( body_b_id ));

	if( body_a == 0 || body_b == 0 )
		fprintf( stderr, "ConstraintEntity::FinalizeLoadElements() -> body_a or body_b was null!\n" );
	else
		Attach( body_a, body_b, offset_a, offset_b );
}
