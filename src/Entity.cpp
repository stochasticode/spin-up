#include <GL/gl.h>
#include <Entity.h>
#include <SpinGame.h>
#include <SpinXML.h>
#include <chipmunk.h>
#include <tinyxml.h>
#include <sstream>
#include <cstdio>

using namespace spin;

void QuadEntity::Render()
{
	glPushMatrix();
	glTranslatef( position.x, position.y, 0 );
	glRotatef( rotation, 0, 0, 1 );
	glScalef( scale, scale, 1 );

	for( int i = 0; i < quads.size(); i++ )
		quads[i].Render();

	glPopMatrix();
}

bool QuadEntity::LoadXML( TiXmlElement* element )
{
	// iterate through children
	TiXmlElement* child = element->FirstChildElement();
	while( child != 0 )
	{
		// vec2d
		if( strcmp( "vec2d", child->Value() ) == 0 )
		{
			std::string name = "";
			Vector vec2d;
			if( !SpinXML::ReadVec2D( child, name, vec2d ) )
			{
				fprintf( stderr, "Quad::LoadXML -> ReadVec2D failed!\n" );
				return false;
			}

			// position
			if( name.compare( "position" ) == 0 )
				position = vec2d;
			// unsupported
			else
				fprintf( stderr, "QuadEntity::LoadXML -> unsuppored vec2d: %s!\n", name.c_str() );
		}


		// quad 
		else if( strcmp( "quad", child->Value() ) == 0 )
		{
			Quad new_quad;
			if( !new_quad.LoadXML( child ) )
			{
				fprintf( stderr, "BodyEntity::LoadXML -> Quad::LoadXML failed!\n" );
				return false;
			}
			quads.push_back( new_quad );
		}
		child = child->NextSiblingElement();
	}
	return true;
}

SurfaceEntity::SurfaceEntity( float x1, float y1, float x2, float y2, float new_radius, float friction ): Entity(), radius( new_radius )
{
	shape = (cpSegmentShape*)cpSegmentShapeNew( 0, cpv( x1, y1 ), cpv( x2, y2 ), radius );
	shape->shape.collision_type = World::COL_TYPE_SURFACE;
	cpShapeSetFriction( (cpShape*)shape, friction );
	shape->shape.body = SPIN.world.GetCPSpace()->staticBody;
	cpSpaceAddShape( SPIN.world.GetCPSpace(), (cpShape*)shape );
}

SurfaceEntity::~SurfaceEntity()
{
	if( shape != 0 )
	{
		cpSpaceRemoveShape( SPIN.world.GetCPSpace(), (cpShape*)shape );
		cpShapeFree( (cpShape*)shape );
		shape = 0;
	}
}

void SurfaceEntity::Render()
{
	glDisable( GL_TEXTURE_2D );
	glBegin(GL_LINES);
		glColor3f( 1, 1, 1 );
		glVertex3f( shape->a.x, shape->a.y, 0 );
		glVertex3f( shape->b.x, shape->b.y, 0 );
	glEnd();
	glEnable( GL_TEXTURE_2D );
}

ConstraintEntity::ConstraintEntity(): Entity(), constraint( 0 )
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

void ConstraintEntity::Render()
{
}

// slide joint between two bodies
void ConstraintEntity::InitConstraintSlideJoint( BodyEntity* new_body_a, BodyEntity* new_body_b, Vector offset_a, Vector offset_b, float min_length, float max_length )
{
	ChipmunkCleanup();
	if( new_body_a != 0 && new_body_a->body != 0 && new_body_b != 0 && new_body_b->body != 0 )
	{
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpSlideJointNew( new_body_a->body, new_body_b->body, cpv( offset_a.x, offset_a.y ), cpv( offset_b.x, offset_b.y ), min_length, max_length ) );
		new_body_a->constraint_ids.push_back( id );
		new_body_b->constraint_ids.push_back( id );
	}
	else
		fprintf( stderr, "ConstraintEntity::InitConstraintSlideJoint -> failed due to null pointer!\n" );
}

// slide joint between body and static body
void ConstraintEntity::InitConstraintSlideJoint( BodyEntity* new_body_a, Vector static_anchor, float min_length, float max_length )
{
	ChipmunkCleanup();
	if( new_body_a != 0 && new_body_a->body != 0 )
	{
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpSlideJointNew( new_body_a->body, SPIN.world.GetCPSpace()->staticBody, cpvzero, cpv( static_anchor.x, static_anchor.y ), min_length, max_length ) );
		new_body_a->constraint_ids.push_back( id );
	}
	else
		fprintf( stderr, "ConstraintEntity::InitConstraintSlideJoint -> failed due to null pointer!\n" );
}

// spring between two bodies
void ConstraintEntity::InitConstraintSpring( BodyEntity* new_body_a, BodyEntity* new_body_b, Vector offset_a, Vector offset_b, float length, float strength, float damping )
{
	ChipmunkCleanup();
	if( new_body_a != 0 && new_body_a->body != 0 && new_body_b != 0 && new_body_b->body != 0 )
	{
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpDampedSpringNew( new_body_a->body, new_body_b->body, cpv( offset_a.x, offset_a.y ), cpv( offset_b.x, offset_b.y ), length, strength, damping ) );
		new_body_a->constraint_ids.push_back( id );
		new_body_b->constraint_ids.push_back( id );
	}
	else
		fprintf( stderr, "ConstraintEntity::InitConstraintSpring -> failed due to null pointer!\n" );
}

// spring between body and static body
void ConstraintEntity::InitConstraintSpring( BodyEntity* new_body_a, Vector static_anchor, float length, float strength, float damping )
{
	ChipmunkCleanup();
	if( new_body_a != 0 && new_body_a->body != 0 )
	{
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpDampedSpringNew( new_body_a->body, SPIN.world.GetCPSpace()->staticBody, cpvzero, cpv( static_anchor.x, static_anchor.y ), length, strength, damping ) );
		new_body_a->constraint_ids.push_back( id );
	}
	else
		fprintf( stderr, "ConstraintEntity::InitConstraintSpring -> failed due to null pointer!\n" );
}

	/*
bool QuadEntity::ReadQuadEntity( TiXmlElement* element, QuadEntity** entity_out )
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
	*/
