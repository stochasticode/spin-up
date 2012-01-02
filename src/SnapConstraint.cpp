#include <SnapConstraint.h>
#include <SpinGame.h>
#include <SpinXML.h>
#include <SpinUtil.h>
#include <tinyxml.h>
#include <stdio.h>

using namespace spin;

SnapConstraint::SnapConstraint( float new_length, float new_strength, float new_damping, float new_max_length ): ConstraintSpring( new_length, new_strength, new_damping ), max_length( new_max_length )
{
}

void SnapConstraint::Render()
{
	if( constraint != 0 )
	{
		cpDampedSpring* spring = (cpDampedSpring*)constraint;
		cpBody* a = spring->constraint.a;
		cpBody* b = spring->constraint.b;

		float cos_a = cos( a->a );
		float sin_a = sin( a->a );
		float cos_b = cos( b->a );
		float sin_b = sin( b->a );
		float anchor_x = 0.0;
		float anchor_y = 0.0;

		// rope
		glLineWidth( 6.0 );
		glDisable( GL_TEXTURE_2D );
		glBegin( GL_LINES );
			glColor4f( 1.0, 1.0, 1.0, 0.5 );
	
			anchor_x = spring->anchr1.x * cos_a - spring->anchr1.y * sin_a;
			anchor_y = spring->anchr1.x * sin_a + spring->anchr1.y * cos_a;
			glVertex3f( cpBodyGetPos( a ).x + anchor_x, cpBodyGetPos( a ).y + anchor_y, 0 );
	
			anchor_x = spring->anchr2.x * cos_b - spring->anchr2.y * sin_b;
			anchor_y = spring->anchr2.x * sin_b + spring->anchr2.y * cos_b;
			glVertex3f( cpBodyGetPos( b ).x + anchor_x, cpBodyGetPos( b ).y + anchor_y, 0 );
		glEnd();
		glEnable( GL_TEXTURE_2D );
		glLineWidth( 1.0 );
	}
}

void SnapConstraint::Tick( int milliseconds )
{
	if( constraint != 0 )
	{
		cpDampedSpring* spring = (cpDampedSpring*)constraint;
		cpBody* a = spring->constraint.a;
		cpBody* b = spring->constraint.b;

		float cos_a = cos( a->a );
		float sin_a = sin( a->a );
		float cos_b = cos( b->a );
		float sin_b = sin( b->a );
		float anchor_x = 0.0;
		float anchor_y = 0.0;

		anchor_x = spring->anchr1.x * cos_a - spring->anchr1.y * sin_a;
		anchor_y = spring->anchr1.x * sin_a + spring->anchr1.y * cos_a;
		float body_a_x = cpBodyGetPos( a ).x + anchor_x;
		float body_a_y = cpBodyGetPos( a ).y + anchor_y;

		anchor_x = spring->anchr2.x * cos_b - spring->anchr2.y * sin_b;
		anchor_y = spring->anchr2.x * sin_b + spring->anchr2.y * cos_b;
		float body_b_x = cpBodyGetPos( b ).x + anchor_x;
		float body_b_y = cpBodyGetPos( b ).y + anchor_y;

		float length = sqrt( (body_a_x-body_b_x)*(body_a_x-body_b_x) + (body_a_y-body_b_y)*(body_a_y-body_b_y) );
		if( length > max_length )
			dead = true;
	}
}

bool SnapConstraint::TryLoadElement( TiXmlElement* element, bool& error )
{
	/*
	// first child must be an entity_ref to a BodyEntity
	Entity* entity_a = 0;
	TiXmlElement* child1 = element->FirstChildElement();
	if( child1 == 0 )
	{
		fprintf( stderr, "SpinXML::ReadSnapConstraint -> constraint has no child elements!'\n" );
		error = true;
		return false;
	}
	if( strcmp( "entity_ref", child1->Value() ) != 0 )
	{
		fprintf( stderr, "SpinXML::ReadSnapConstraint -> first child was not an entity_ref!'\n" );
		error = true;
		return false;
	}
	std::string alias1( child1->Attribute( "entity_alias" ) );
	entity_a = SPIN.world.GetEntityByAlias( alias1 );
	if( !entity_a )
	{
		fprintf( stderr, "SpinXML::ReadSnapConstraint -> could not find entity with alias: %s!'\n", alias1.c_str() );
		error = true;
		return false;
	}
	// make sure it is a BodyEntity
	BodyEntity* body_a = dynamic_cast<BodyEntity*>(entity_a);
	if( body_a == 0 )
	{
		fprintf( stderr, "SpinXML::ReadSnapConstraint -> entity with alias: %s is not a BodyEntity!'\n", alias1.c_str() );
		error = true;
		return false;
	}

	// for now second child needs to be a vec2d
	Vector static_anchor;
	TiXmlElement* child2 = child1->NextSiblingElement();
	if( child2 == 0 )
	{
		fprintf( stderr, "SpinXML::ReadSnapConstraint -> constraint has only one child element!'\n" );
		error = true;
		return false;
	}
	std::string name;
	if( strcmp( "vec2d", child2->Value() ) != 0 || !SpinXML::ReadVec2D( child2, name, static_anchor) )
	{
		fprintf( stderr, "SpinXML::ReadSnapConstraint -> ReadVec2D failed for second child element!'\n" );
		error = true;
		return false;
	}

	// iterate through rest of children
	TiXmlElement* next_child = child2->NextSiblingElement();
	while( next_child != 0 )
	{
		// param
		if( strcmp( "param", next_child->Value() ) == 0 )
		{
			std::string name = "";
			std::string value = "";
			SpinXML::ReadParam( next_child, name, value );

			// strength
			if( name.compare( "strength" ) == 0 )
			{
				if( !SpinUtil::ToFloat( value.c_str(), strength) )
				{
					fprintf( stderr, "SnapConstraint::LoadXML -> invalid strength value: %s\n", value.c_str() );
					return false;
				}
			}
			// damping
			if( name.compare( "damping" ) == 0 )
			{
				if( !SpinUtil::ToFloat( value.c_str(), damping) )
				{
					fprintf( stderr, "SnapConstraint::LoadXML -> invalid damping value: %s\n", value.c_str() );
					return false;
				}
			}
			// max_length
			else if( name.compare( "max_length" ) == 0 )
			{
				if( !SpinUtil::ToFloat( value.c_str(), max_length) )
				{
					fprintf( stderr, "SnapConstraint::LoadXML -> invalid max_length value: %s\n", value.c_str() );
					return false;
				}
			}
			// unsupported
			else
			{
				fprintf( stderr, "SnapConstraint::LoadXML -> unsupported param: %s\n", name.c_str() );
			}
		}

		next_child = next_child->NextSiblingElement();
	}

	Attach( body_a, SPIN.world.GetStaticBody(), Vector( 0, 0 ), static_anchor );
	return true;
	*/
	return true;
}
