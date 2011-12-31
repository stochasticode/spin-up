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

/*
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
*/
